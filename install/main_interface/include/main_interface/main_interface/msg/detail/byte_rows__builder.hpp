// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from main_interface:msg/ByteRows.idl
// generated code does not contain a copyright notice

#ifndef MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__BUILDER_HPP_
#define MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "main_interface/msg/detail/byte_rows__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace main_interface
{

namespace msg
{

namespace builder
{

class Init_ByteRows_rows
{
public:
  Init_ByteRows_rows()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::main_interface::msg::ByteRows rows(::main_interface::msg::ByteRows::_rows_type arg)
  {
    msg_.rows = std::move(arg);
    return std::move(msg_);
  }

private:
  ::main_interface::msg::ByteRows msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::main_interface::msg::ByteRows>()
{
  return main_interface::msg::builder::Init_ByteRows_rows();
}

}  // namespace main_interface

#endif  // MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__BUILDER_HPP_
