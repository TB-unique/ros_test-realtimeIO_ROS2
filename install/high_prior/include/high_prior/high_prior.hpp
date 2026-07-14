#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/float64.hpp>


class HighPrior : public rclcpp::Node
{
public:
  explicit HighPrior(const rclcpp::NodeOptions & options);

private:
  rclcpp::CallbackGroup::SharedPtr sub_cbg_;
  rclcpp::CallbackGroup::SharedPtr timer_cbg_;
  
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr cmd_pub_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr state_sub_;
  
  double target_position_{1.0};
  bool rt_configured_{false};
};

