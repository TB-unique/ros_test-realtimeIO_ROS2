// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from main_interface:msg/ByteRow.idl
// generated code does not contain a copyright notice

#ifndef MAIN_INTERFACE__MSG__DETAIL__BYTE_ROW__BUILDER_HPP_
#define MAIN_INTERFACE__MSG__DETAIL__BYTE_ROW__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "main_interface/msg/detail/byte_row__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace main_interface
{

namespace msg
{

namespace builder
{

class Init_ByteRow_data
{
public:
  Init_ByteRow_data()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::main_interface::msg::ByteRow data(::main_interface::msg::ByteRow::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::main_interface::msg::ByteRow msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::main_interface::msg::ByteRow>()
{
  return main_interface::msg::builder::Init_ByteRow_data();
}

}  // namespace main_interface

#endif  // MAIN_INTERFACE__MSG__DETAIL__BYTE_ROW__BUILDER_HPP_
