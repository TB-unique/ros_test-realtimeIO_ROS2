// src/IOqueue.cpp
#include "IOqueue/IOqueue.hpp"
#include "realtime_common/rt_utils.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <random>

namespace io_queue
{

IOqueue::IOqueue(const rclcpp::NodeOptions & options)
: Node("io_bridge_component", options)
{
  // 声明参数
  declare_parameter("read_io_priority", 90);
  declare_parameter("write_io_priority", 90);
  declare_parameter("read_cpu_core", 2);
  declare_parameter("write_cpu_core", 2);
  declare_parameter("read_pub_cpu_core", 3);
  declare_parameter("io_period_ms_read", 0.02);
  declare_parameter("io_period_ms_write", 0.02);
  declare_parameter("read_file", "/tmp/loopback_input.bin");
  declare_parameter("write_file", "/tmp/loopback_output.bin");
  
  get_parameter("read_io_priority", read_io_priority_);
  get_parameter("write_io_priority", write_io_priority_);
  get_parameter("read_cpu_core", read_cpu_core_);
  get_parameter("write_cpu_core", write_cpu_core_);
  // get_parameter("read_pub_cpu_core", read_pub_cpu_core_);
  get_parameter("io_period_ms_read", io_period_ms_read);
  get_parameter("io_period_ms_write", io_period_ms_write);
  io_read_file_path_ = get_parameter("read_file").as_string();
  io_write_file_path_ = get_parameter("write_file").as_string();
  
  // 创建 ROS 接口
  write_sub_ = create_subscription<main_interface::msg::ByteRow>(
    "/command_data", 1000,
    std::bind(&IOqueue::rosTopic_to_write_queue, this, std::placeholders::_1));
  
  read_pub_ = create_publisher<main_interface::msg::ByteRows>(
    "/read_pub", 1000);

  // 打开读文件：read_hardware 从此文件读取数据
  io_read_file_.open(io_read_file_path_, std::ios::binary);
  if (!io_read_file_.is_open()) {
    RCLCPP_ERROR(get_logger(), "Failed to open read file: %s",
                 io_read_file_path_.c_str());
  } else {
    RCLCPP_INFO(get_logger(), "Opened read file: %s",
                io_read_file_path_.c_str());
  }

  // 打开写文件：write_hardware 将数据写入此文件
  io_write_file_.open(io_write_file_path_, std::ios::binary);
  if (!io_write_file_.is_open()) {
    RCLCPP_ERROR(get_logger(), "Failed to open write file: %s",
                 io_write_file_path_.c_str());
  } else {
    RCLCPP_INFO(get_logger(), "Opened write file: %s",
                io_write_file_path_.c_str());
  }
  
  // 启动实时线程
  running_ = true;

  // 写 IO 线程（稍低优先级）
  write_io_thread_ = std::thread([this]() {
    realtime_common::set_cpu_affinity(write_cpu_core_);
    realtime_common::set_realtime_priority(write_io_priority_);
    write_io_thread();
  });
  
  read_queue_to_rosTopic_thread_ = std::thread([this]() {
    realtime_common::set_cpu_affinity(read_cpu_core_);
    realtime_common::set_realtime_priority(read_io_priority_);
    read_queue_to_rosTopic();
  });

  // 读 IO 线程（高优先级）
  read_io_thread_ = std::thread([this]() {
    realtime_common::set_cpu_affinity(read_cpu_core_);
    realtime_common::set_realtime_priority(read_io_priority_);
    read_io_thread();
  });
  
  RCLCPP_INFO(get_logger(), "IOqueue Component started");
  RCLCPP_INFO(get_logger(), "Read IO: CPU%d P%d, Pub CPU%d, Period %.3fms",
              read_cpu_core_, read_io_priority_, read_cpu_core_, io_period_ms_read);
  RCLCPP_INFO(get_logger(), "Write IO: CPU%d P%d, Period %.3fms",
              write_cpu_core_, write_io_priority_, io_period_ms_write);
  RCLCPP_INFO(get_logger(), "Read file: %s", io_read_file_path_.c_str());
  RCLCPP_INFO(get_logger(), "Write file: %s", io_write_file_path_.c_str());
}

IOqueue::~IOqueue()
{
  RCLCPP_INFO(get_logger(), "Shutting down IOqueue Component...");

  running_ = false;
  
  if (read_io_thread_.joinable()) {
    read_io_thread_.join();
  }

  if (read_queue_to_rosTopic_thread_.joinable()) {
    read_queue_to_rosTopic_thread_.join();
  }
  
  if (write_io_thread_.joinable()) {
    write_io_thread_.join();
  }

  // 关闭文件
  if (io_read_file_.is_open()) {
    io_read_file_.close();
  }
  if (io_write_file_.is_open()) {
    io_write_file_.close();
  }
  
  RCLCPP_INFO(get_logger(), "Component stopped. Read: %lu, Write: %lu",
              read_count_.load(), write_count_.load());

  // 校验输入输出文件
  try {
    auto read_size = std::filesystem::file_size(io_read_file_path_);
    auto write_size = std::filesystem::file_size(io_write_file_path_);
    RCLCPP_INFO(get_logger(), "=== File IO Test Complete ===");
    RCLCPP_INFO(get_logger(), "Read  file (%s): %lu bytes",
                io_read_file_path_.c_str(), read_size);
    RCLCPP_INFO(get_logger(), "Write file (%s): %lu bytes",
                io_write_file_path_.c_str(), write_size);

    if (read_size == write_size) {
      std::ifstream fin(io_read_file_path_, std::ios::binary);
      std::ifstream fout(io_write_file_path_, std::ios::binary);
      uint64_t diff_count = 0;
      char a, b;
      while (fin.get(a) && fout.get(b)) {
        if (a != b) diff_count++;
      }
      if (diff_count == 0) {
        RCLCPP_INFO(get_logger(), "VERIFY: PASS - files are identical");
      } else {
        RCLCPP_WARN(get_logger(), "VERIFY: FAIL - %lu bytes differ", diff_count);
      }
    } else {
      RCLCPP_WARN(get_logger(),
        "VERIFY: FAIL - size mismatch (read=%lu, write=%lu)", read_size, write_size);
    }
  } catch (const std::exception & e) {
    RCLCPP_ERROR(get_logger(), "Verification error: %s", e.what());
  }
}

// === ROS 订阅回调（非实时，只做数据拷贝）===
void IOqueue::rosTopic_to_write_queue(
  const main_interface::msg::ByteRow::SharedPtr msg)
{
  // LoopbackNode 聚合后可能产生超大消息，按 MAX_MSG_SIZE 分片推入 write_queue
  const uint8_t * src = msg->data.data();
  std::size_t remaining = msg->data.size();

  while (remaining > 0) {
    QueueSlot slot;
    slot.size = std::min(remaining, MAX_MSG_SIZE);
    std::memcpy(slot.data.data(), src, slot.size);

    if (!write_queue_.try_push(slot)) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                           "Write queue full, dropping data");
      return;  // 队列满，丢弃剩余分片
    }

    src += slot.size;
    remaining -= slot.size;
  }
}

// === 读 IO 线程（实时）===
void IOqueue::read_io_thread()
{
  using clock = std::chrono::steady_clock;
  auto next_cycle = clock::now();
  const auto period = std::chrono::microseconds(static_cast<int64_t>(io_period_ms_read * 1000));
  
  main_interface::msg::ByteRow read_data;
  main_interface::msg::ByteRows ros_msg;
  
  while (running_) {

    //next_cycle = clock::now();
    auto start = clock::now();
    
    // 1. 读取硬件 IO（非阻塞，最坏情况 < 500μs）
    if (read_hardware(read_data)) {
      // 2. ByteRow → QueueSlot，推入队列供 ROS 发布
      QueueSlot slot;
      slot.size = std::min(read_data.data.size(), MAX_MSG_SIZE);
      std::memcpy(slot.data.data(), read_data.data.data(), slot.size);

      if (!read_queue_.try_push(slot)) {
        RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                             "Read queue full, dropping data");
      }
      
      read_count_++;
    } else {
      // 文件已读完，退出读 IO 线程
      RCLCPP_INFO_ONCE(get_logger(), "Read file complete, exiting read_io_thread");
     // break;
    }
    
    // // 3. 检查抖动
    // auto end = clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    // auto expected = static_cast<int64_t>(io_period_ms_read * 1000);
    // int64_t jitter = abs(duration - expected);
    
    // int64_t old_max = max_read_jitter_us_.load();
    // while (old_max < jitter && 
    //        !max_read_jitter_us_.compare_exchange_weak(old_max, jitter));
    
    // 4. 精确周期等待
    next_cycle += period;
    std::this_thread::sleep_until(next_cycle);
  }
}

void IOqueue::read_queue_to_rosTopic()
{
  using clock = std::chrono::steady_clock;
  auto next_cycle = clock::now();
  const auto period = std::chrono::microseconds(static_cast<int64_t>(2 * 1000)); // 2ms 发布周期

  while (running_) {
    main_interface::msg::ByteRows ros_msg;
    QueueSlot slot;

    // 批量出队，QueueSlot → ByteRow
    while (read_queue_.try_pop(slot)) {
      main_interface::msg::ByteRow row;
      row.data.assign(slot.data.begin(), slot.data.begin() + slot.size);
      ros_msg.rows.push_back(std::move(row));
    }
    
    if (!ros_msg.rows.empty()) {
      read_pub_->publish(ros_msg);
    }

    next_cycle += period;
    std::this_thread::sleep_until(next_cycle);
  }
}

// === 写 IO 线程（实时）===
void IOqueue::write_io_thread()
{
  using clock = std::chrono::steady_clock;
  auto next_cycle = clock::now();
  const auto period = std::chrono::microseconds(static_cast<int64_t>(io_period_ms_write * 1000));
  
  main_interface::msg::ByteRow cmd_data;
  
  while (running_) {
    auto start = clock::now();
    
    // 1. 从队列取命令（非阻塞）
    QueueSlot slot;
    if (write_queue_.try_pop(slot)) {
      // QueueSlot → ByteRow
      cmd_data.data.assign(slot.data.begin(), slot.data.begin() + slot.size);

      // 2. 写入硬件 IO（非阻塞，最坏情况 < 300μs）
      if (!write_hardware(cmd_data)) {
        RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                             "Hardware write failed");
      }
      
      write_count_++;
    }
    
    // // 3. 检查抖动
    // auto end = clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    // auto expected = static_cast<int64_t>(io_period_ms_write * 1000);
    // int64_t jitter = abs(duration - expected);
    
    // int64_t old_max = max_write_jitter_us_.load();
    // while (old_max < jitter && 
    //        !max_write_jitter_us_.compare_exchange_weak(old_max, jitter));
    
    // 4. 精确周期等待
    next_cycle += period;
    std::this_thread::sleep_until(next_cycle);
  }
}

// === 硬件 IO 接口（文件模拟）===
bool IOqueue::read_hardware(main_interface::msg::ByteRow & data)
{
  // 从输入文件读取数据（模拟从硬件读取传感器/编码器数据）
  if (read_complete_.load()) {
    return false;  // 文件已读完
  }

  if (!io_read_file_.is_open() || !io_read_file_.good()) {
    read_complete_ = true;
    return false;
  }

  // 每次随机读取 500~64KB，模拟真实硬件 IO 数据量不均的特性
  thread_local std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<std::size_t> dist(500, 1024*1024);
  const std::size_t read_size = dist(rng);
  data.data.resize(read_size);

  io_read_file_.read(reinterpret_cast<char *>(data.data.data()), read_size);
  auto bytes_read = static_cast<std::size_t>(io_read_file_.gcount());

  if (bytes_read == 0) {
    // 文件读完，停止读取
    read_complete_ = true;
    RCLCPP_INFO(get_logger(), "Read file complete, total reads: %lu",
                read_count_.load());
    return false;
  }

  data.data.resize(bytes_read);
  return true;
}

bool IOqueue::write_hardware(const main_interface::msg::ByteRow & data)
{
  // 将数据写入输出文件（模拟写入硬件寄存器/DAC）
  if (!io_write_file_.is_open()) {
    return false;
  }

  io_write_file_.write(
    reinterpret_cast<const char *>(data.data.data()),
    data.data.size());
  io_write_file_.flush();

  return io_write_file_.good();
}

}  // namespace io_queue

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(io_queue::IOqueue)