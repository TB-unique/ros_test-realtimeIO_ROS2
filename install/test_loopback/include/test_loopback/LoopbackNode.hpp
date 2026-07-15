#pragma once

#include <rclcpp/rclcpp.hpp>
#include "main_interface/msg/byte_row.hpp"
#include "main_interface/msg/byte_rows.hpp"

#include <atomic>
#include <thread>

namespace test_loopback
{

class LoopbackNode : public rclcpp::Node
{
public:
  explicit LoopbackNode(const rclcpp::NodeOptions & options);
  virtual ~LoopbackNode();

private:
  // 订阅 /read_pub（IOqueue 从文件读出的数据），转发到 /command_data
  void read_callback(const main_interface::msg::ByteRows::SharedPtr msg);

  // 统计打印线程
  void stats_thread_func();

  // ROS 接口
  rclcpp::Subscription<main_interface::msg::ByteRows>::SharedPtr read_sub_;
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

}  // namespace test_loopback
