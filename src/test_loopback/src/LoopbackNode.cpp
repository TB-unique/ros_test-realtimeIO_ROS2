#include "test_loopback/LoopbackNode.hpp"

#include <chrono>

namespace test_loopback
{

LoopbackNode::LoopbackNode(const rclcpp::NodeOptions & options)
: Node("loopback_node", options)
{
  // 订阅 IOqueue 从文件读取后发布的数据
  read_sub_ = create_subscription<main_interface::msg::ByteRows>(
    "/read_pub", 1000,
    std::bind(&LoopbackNode::read_callback, this, std::placeholders::_1));

  // 将数据转发回 IOqueue（write_hardware 会写入输出文件）
  command_pub_ = create_publisher<main_interface::msg::ByteRow>(
    "/command_data", 1000);

  // 启动统计线程
  running_ = true;
  stats_thread_ = std::thread(&LoopbackNode::stats_thread_func, this);

  RCLCPP_INFO(get_logger(),
    "LoopbackNode started: /read_pub -> /command_data (forward only)");
}

LoopbackNode::~LoopbackNode()
{
  RCLCPP_INFO(get_logger(), "Shutting down LoopbackNode...");

  running_ = false;
  if (stats_thread_.joinable()) {
    stats_thread_.join();
  }

  RCLCPP_INFO(get_logger(),
    "LoopbackNode stopped. Received: %lu msgs/%lu B, Forwarded: %lu msgs/%lu B",
    msgs_received_.load(), bytes_received_.load(),
    msgs_forwarded_.load(), bytes_forwarded_.load());
}

void LoopbackNode::read_callback(const main_interface::msg::ByteRows::SharedPtr msg)
{
  msgs_received_ += msg->rows.size();

  // 将接收到的所有行拼接成一包 ByteRow 发送
  auto cmd_msg = std::make_unique<main_interface::msg::ByteRow>();

  // 预计算总大小，避免 vector 反复 reallocation
  std::size_t total_size = 0;
  for (const auto & row : msg->rows) {
    if (row.data.empty()) continue;
    total_size += row.data.size();
  }
  cmd_msg->data.reserve(total_size);

  for (const auto & row : msg->rows) {
    if (row.data.empty()) continue;

    cmd_msg->data.insert(cmd_msg->data.end(), row.data.begin(), row.data.end());
    bytes_received_ += row.data.size();
  }

  // 避免发布空消息
  if (cmd_msg->data.empty()) return;

  bytes_forwarded_ += cmd_msg->data.size();
  msgs_forwarded_++;
  command_pub_->publish(std::move(cmd_msg));
}

void LoopbackNode::stats_thread_func()
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

}  // namespace test_loopback

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(test_loopback::LoopbackNode)
