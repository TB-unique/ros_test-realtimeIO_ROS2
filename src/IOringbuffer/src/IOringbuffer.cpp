#include "IOringbuffer/IOringbuffer.hpp"
#include "realtime_common/rt_utils.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <random>

namespace io_ringbuffer
{

IOringbuffer::IOringbuffer(const rclcpp::NodeOptions & options)
: rclcpp::Node("io_ringbuffer", options)
{
  // 声明参数
  declare_parameter("read_io_priority", 90);
  declare_parameter("write_io_priority", 90);
  declare_parameter("read_cpu_core", 2);
  declare_parameter("write_cpu_core", 2);
  // declare_parameter("read_pub_cpu_core", 3);
  declare_parameter("io_period_ms_read", 0.02);
  declare_parameter("io_period_ms_write", 0.02);
  declare_parameter("read_file", "/tmp/ringbuffer_input.bin");
  declare_parameter("write_file", "/tmp/ringbuffer_output.bin");

  get_parameter("read_io_priority", read_io_priority_);
  get_parameter("write_io_priority", write_io_priority_);
  get_parameter("read_cpu_core", read_cpu_core_);
  get_parameter("write_cpu_core", write_cpu_core_);
  // get_parameter("read_pub_cpu_core", read_pub_cpu_core_);
  get_parameter("io_period_ms_read", io_period_ms_read_);
  get_parameter("io_period_ms_write", io_period_ms_write_);
  io_read_file_path_ = get_parameter("read_file").as_string();
  io_write_file_path_ = get_parameter("write_file").as_string();

  // 创建 ROS 接口
  write_sub_ = this->create_subscription<main_interface::msg::ByteRow>(
      "input_topic", 1000,
      std::bind(&IOringbuffer::rosTopic_to_write_ringbuffer, this, std::placeholders::_1));

  read_pub_ = this->create_publisher<main_interface::msg::ByteRow>(
      "output_topic", 1000);

  // 打开读文件
  io_read_file_.open(io_read_file_path_, std::ios::binary);
  if (!io_read_file_.is_open()) {
    RCLCPP_ERROR(get_logger(), "Failed to open read file: %s",
                 io_read_file_path_.c_str());
  } else {
    RCLCPP_INFO(get_logger(), "Opened read file: %s",
                io_read_file_path_.c_str());
  }

  // 打开写文件
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

  // 写 IO 线程
  write_io_thread_ = std::thread([this]() {
    realtime_common::set_cpu_affinity(write_cpu_core_);
    realtime_common::set_realtime_priority(write_io_priority_);
    write_io_thread();
  });

  // 消费者发布线程（独立核心）
  read_ringbuffer_to_rosTopic_thread_ = std::thread([this]() {
    realtime_common::set_cpu_affinity(read_cpu_core_);
    realtime_common::set_realtime_priority(read_io_priority_);
    read_ringbuffer_to_rosTopic();
  });

  // 读 IO 线程（高优先级）
  read_io_thread_ = std::thread([this]() {
    realtime_common::set_cpu_affinity(read_cpu_core_);
    realtime_common::set_realtime_priority(read_io_priority_);
    read_io_thread();
  });

  RCLCPP_INFO(get_logger(), "IOringbuffer Component started");
  RCLCPP_INFO(get_logger(), "Read IO: CPU%d P%d, Pub CPU%d, Period %.3fms",
              read_cpu_core_, read_io_priority_, read_cpu_core_, io_period_ms_read_);
  RCLCPP_INFO(get_logger(), "Write IO: CPU%d P%d, Period %.3fms",
              write_cpu_core_, write_io_priority_, io_period_ms_write_);
  RCLCPP_INFO(get_logger(), "Read file: %s", io_read_file_path_.c_str());
  RCLCPP_INFO(get_logger(), "Write file: %s", io_write_file_path_.c_str());
}

IOringbuffer::~IOringbuffer()
{
  RCLCPP_INFO(get_logger(), "Shutting down IOringbuffer Component...");

  running_ = false;

  if (read_io_thread_.joinable()) {
    read_io_thread_.join();
  }

  if (read_ringbuffer_to_rosTopic_thread_.joinable()) {
    read_ringbuffer_to_rosTopic_thread_.join();
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
    RCLCPP_INFO(get_logger(), "=== Ringbuffer IO Test Complete ===");
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
void IOringbuffer::rosTopic_to_write_ringbuffer(
    const main_interface::msg::ByteRow::SharedPtr msg)
{

   const uint8_t * src = msg->data.data();
  std::size_t remaining = msg->data.size();

  while (remaining > 0) {
    RingBufferSlot slot;
    slot.size = std::min(remaining, MAX_MSG_SIZE);
    std::memcpy(slot.data.data(), src, slot.size);

    if (!write_ringbuffer_.try_push(slot)) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                           "Write ringbuffer is full, discard message");
      break; // 如果环形缓冲区满了，丢弃剩余数据
    }

    src += slot.size;
    remaining -= slot.size;
  }
}

// === 读 IO 线程（实时）===
void IOringbuffer::read_io_thread()
{
  using clock = std::chrono::steady_clock;
  auto next_cycle = clock::now();
  const auto period = std::chrono::microseconds(static_cast<int64_t>(io_period_ms_read_ * 1000));

  main_interface::msg::ByteRow read_data;

  while (running_) {
    // 1. 读取硬件 IO
    if (read_hardware(read_data)) {
      // 2. ByteRow → RingBufferSlot，推入 ringbuffer 供 ROS 发布
      RingBufferSlot slot;
      slot.size = std::min(read_data.data.size(), MAX_MSG_SIZE);
      std::memcpy(slot.data.data(), read_data.data.data(), slot.size);

      if (!read_ringbuffer_.try_push(slot)) {
        RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                             "Read ringbuffer full, dropping data");
      }

      read_count_++;
    } else {
      // 文件已读完，退出读 IO 线程
      RCLCPP_INFO_ONCE(get_logger(), "Read file complete, exiting read_io_thread");
      //break;
    }

    // 3. 精确周期等待
    next_cycle += period;
    std::this_thread::sleep_until(next_cycle);
  }
}

void IOringbuffer::read_ringbuffer_to_rosTopic()
{
  using clock = std::chrono::steady_clock;
  auto next_cycle = clock::now();
  const auto period = std::chrono::microseconds(static_cast<int64_t>(2 * 1000)); // 2ms 发布周期

  while (running_) {
    main_interface::msg::ByteRow ros_msg;
    RingBufferSlot slot;

    // 批量出队并拼接后发布
    while (read_ringbuffer_.try_pop(slot)) {
      ros_msg.data.insert(ros_msg.data.end(),
                          slot.data.begin(),
                          slot.data.begin() + slot.size);
    }

    if (!ros_msg.data.empty()) {
      read_pub_->publish(std::move(ros_msg));
    }

    next_cycle += period;
    std::this_thread::sleep_until(next_cycle);
  }
}

void IOringbuffer::write_io_thread()
{
  using clock = std::chrono::steady_clock;
  auto next_cycle = clock::now();
  const auto period = std::chrono::microseconds(static_cast<int64_t>(io_period_ms_write_ * 1000));

  RingBufferSlot slot;
  while (running_) {
    // 从环形缓冲区读取数据
    if (write_ringbuffer_.try_pop(slot)) {
      // RingBufferSlot → ByteRow
      main_interface::msg::ByteRow write_data;
      write_data.data.assign(slot.data.begin(), slot.data.begin() + slot.size);

      if (!write_hardware(write_data)) {
        RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                             "Hardware write failed");
      }

      write_count_++;
    }

    next_cycle += period;
    std::this_thread::sleep_until(next_cycle);
  }
}

// === 硬件 IO 接口（文件模拟）===
bool IOringbuffer::read_hardware(main_interface::msg::ByteRow & data)
{
  if (read_complete_.load()) {
    return false;
  }

  if (!io_read_file_.is_open() || !io_read_file_.good()) {
    read_complete_ = true;
    return false;
  }

  // 每次随机读取 1~1024 字节，模拟真实硬件 IO
  thread_local std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<std::size_t> dist(1, MAX_MSG_SIZE);
  const std::size_t read_size = dist(rng);
  data.data.resize(read_size);

  io_read_file_.read(reinterpret_cast<char *>(data.data.data()), read_size);
  auto bytes_read = static_cast<std::size_t>(io_read_file_.gcount());

  if (bytes_read == 0) {
    read_complete_ = true;
    RCLCPP_INFO(get_logger(), "Read file complete, total reads: %lu",
                read_count_.load());
    return false;
  }

  data.data.resize(bytes_read);
  return true;
}

bool IOringbuffer::write_hardware(const main_interface::msg::ByteRow & data)
{
  if (!io_write_file_.is_open()) {
    return false;
  }

  io_write_file_.write(
    reinterpret_cast<const char *>(data.data.data()),
    data.data.size());
  io_write_file_.flush();

  return io_write_file_.good();
}

}  // namespace io_ringbuffer

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(io_ringbuffer::IOringbuffer)
