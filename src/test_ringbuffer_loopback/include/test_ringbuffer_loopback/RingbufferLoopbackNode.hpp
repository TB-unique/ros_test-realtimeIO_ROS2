#pragma once

#include <rclcpp/rclcpp.hpp>
#include "main_interface/msg/byte_row.hpp"

#include <atomic>
#include <thread>

namespace test_ringbuffer_loopback
{

class RingbufferLoopbackNode : public rclcpp::Node
{
public:
  explicit RingbufferLoopbackNode(const rclcpp::NodeOptions & options);
  virtual ~RingbufferLoopbackNode();

private:
  // 订阅 output_topic（IOringbuffer 从文件读出的数据），转发到 input_topic
  void read_callback(const main_interface::msg::ByteRow::SharedPtr msg);

  // 统计打印线程
  void stats_thread_func();

  // ROS 接口
  rclcpp::Subscription<main_interface::msg::ByteRow>::SharedPtr read_sub_;
  rclcpp::Publisher<main_interface::msg::ByteRow>::SharedPtr command_pub_;

  // 线程控制
  std::thread stats_thread_;
  std::atomic<bool> running_{false};

  // 统计
  std::atomic<uint64_t> msgs_received_{0};
  std::atomic<uint64_t> msgs_forwarded_{0};
  std::atomic<uint64_t> bytes_received_{0};
  std::atomic<uint64_t> bytes_forwarded_{0};
};

}  // namespace test_ringbuffer_loopback
