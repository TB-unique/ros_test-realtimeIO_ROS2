#pragma once

#include <array>
#include <rclcpp/rclcpp.hpp>
#include "main_interface/msg/byte_rows.hpp"
#include "main_interface/msg/byte_row.hpp"
#include "realtime_common/SPSC_ringbuffer.hpp"

#include <fstream>
#include <string>

// 单条消息最大字节数
constexpr std::size_t MAX_MSG_SIZE = 1024*1024;  // 1MB

// 队列容量（必须是2的幂）
// 200MB 文件按平均 2KB 每次读取约 100K 条，消费者 2ms 周期处理，需要足够缓冲
constexpr std::size_t READ_QUEUE_SIZE = 4096;
constexpr std::size_t WRITE_QUEUE_SIZE = 1024;

// 用于 ringbuffer 的固定大小存储单元（trivially copyable）
struct alignas(64) QueueSlot
{
  std::array<uint8_t, MAX_MSG_SIZE> data{};
  std::size_t size{0};
};



namespace io_queue
{

class IOqueue : public rclcpp::Node
{
public:
  explicit IOqueue(const rclcpp::NodeOptions & options);
  virtual ~IOqueue();

private:

  // === 实时线程函数 ===
  void read_io_thread();      // 读 IO → 发布到 ROS
  void write_io_thread();     // 从 ROS 读 → 写 IO
  
  // === 硬件 IO 接口（文件模拟）===
  bool read_hardware(main_interface::msg::ByteRow & data);
  bool write_hardware(const main_interface::msg::ByteRow & data);

  void read_queue_to_rosTopic();
  void rosTopic_to_write_queue(
    const main_interface::msg::ByteRow::SharedPtr msg);
  
  // === 实时线程 ===
  std::thread read_io_thread_;
  std::thread read_queue_to_rosTopic_thread_;
  std::thread write_io_thread_;
  std::atomic<bool> running_{false};
  
  // === ROS 接口 ===
  rclcpp::Subscription<main_interface::msg::ByteRow>::SharedPtr write_sub_;
  rclcpp::Publisher<main_interface::msg::ByteRows>::SharedPtr read_pub_;
  
  // === 无锁环形缓冲区 ===
  realtime_common::SPSCRingBuffer<QueueSlot, READ_QUEUE_SIZE> read_queue_;    // IO → ROS
  realtime_common::SPSCRingBuffer<QueueSlot, WRITE_QUEUE_SIZE> write_queue_;  // ROS → IO
  
  // === 文件IO模拟硬件 ===
  std::string io_read_file_path_;   // read_hardware 读取的输入文件
  std::string io_write_file_path_;  // write_hardware 写入的输出文件
  std::ifstream io_read_file_;      // 读IO文件流（仅 read_io_thread_ 访问）
  std::ofstream io_write_file_;     // 写IO文件流（仅 write_io_thread_ 访问）
  std::atomic<bool> read_complete_{false};  // 文件读取完成标志
  
  // === 参数 ===
  int read_io_priority_{90};
  int write_io_priority_{90};
  int read_cpu_core_{2};
  int write_cpu_core_{2};
  int read_pub_cpu_core_{3};      // 消费者发布线程独立核心
  double io_period_ms_read{0.02};  // 20μs IO读周期
  double io_period_ms_write{0.02}; // 20μs IO写周期
  
  // === 统计 ===
  std::atomic<uint64_t> read_count_{0};
  std::atomic<uint64_t> write_count_{0};
  std::atomic<int64_t> max_read_jitter_us_{0};
  std::atomic<int64_t> max_write_jitter_us_{0};
};

}  // namespace io_queue