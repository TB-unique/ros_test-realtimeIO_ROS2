#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/float64.hpp>


class LowPrior : public rclcpp::Node
{
public:
  explicit LowPrior(const rclcpp::NodeOptions & options);

private:
  rclcpp::CallbackGroup::SharedPtr sub_cbg_;
  rclcpp::CallbackGroup::SharedPtr timer_cbg_;
  
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr cmd_sub_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr state_pub_;
  bool rt_configured_{false};
};

