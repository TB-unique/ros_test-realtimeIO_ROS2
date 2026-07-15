#include "test_ringbuffer_loopback/RingbufferLoopbackNode.hpp"

#include <chrono>

namespace test_ringbuffer_loopback
{

RingbufferLoopbackNode::RingbufferLoopbackNode(const rclcpp::NodeOptions & options)
: Node("ringbuffer_loopback_node", options)
{
  // 订阅 IOringbuffer 发布的数据
  read_sub_ = create_subscription<main_interface::msg::ByteRow>(
    "output_topic", 1000,
    std::bind(&RingbufferLoopbackNode::read_callback, this, std::placeholders::_1));

  // 将数据转发回 IOringbuffer
  command_pub_ = create_publisher<main_interface::msg::ByteRow>(
    "input_topic", 1000);

  // 启动统计线程
  running_ = true;
  stats_thread_ = std::thread(&RingbufferLoopbackNode::stats_thread_func, this);

  RCLCPP_INFO(get_logger(),
    "RingbufferLoopbackNode started: output_topic -> input_topic (passthrough)");
}

RingbufferLoopbackNode::~RingbufferLoopbackNode()
{
  RCLCPP_INFO(get_logger(), "Shutting down RingbufferLoopbackNode...");

  running_ = false;
  if (stats_thread_.joinable()) {
    stats_thread_.join();
  }

  RCLCPP_INFO(get_logger(),
    "RingbufferLoopbackNode stopped. Received: %lu msgs/%lu B, Forwarded: %lu msgs/%lu B",
    msgs_received_.load(), bytes_received_.load(),
    msgs_forwarded_.load(), bytes_forwarded_.load());
}

void RingbufferLoopbackNode::read_callback(const main_interface::msg::ByteRow::SharedPtr msg)
{
  msgs_received_++;
  bytes_received_ += msg->data.size();

  // 直接转发（IOringbuffer 发布的是拼接后的单条 ByteRow，无需再次聚合）
  auto cmd_msg = std::make_unique<main_interface::msg::ByteRow>();
  cmd_msg->data = msg->data;  // 拷贝数据

  bytes_forwarded_ += cmd_msg->data.size();
  msgs_forwarded_++;
  command_pub_->publish(std::move(cmd_msg));
}

void RingbufferLoopbackNode::stats_thread_func()
{
  while (running_) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    if (running_) {
      RCLCPP_INFO(get_logger(),
        "Stats: recv=%lu msgs/%lu B | fwd=%lu msgs/%lu B",
        msgs_received_.load(), bytes_received_.load(),
        msgs_forwarded_.load(), bytes_forwarded_.load());
    }
  }
}

}  // namespace test_ringbuffer_loopback

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(test_ringbuffer_loopback::RingbufferLoopbackNode)
