// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from main_interface:msg/ByteRows.idl
// generated code does not contain a copyright notice

#ifndef MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__TRAITS_HPP_
#define MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "main_interface/msg/detail/byte_rows__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'rows'
#include "main_interface/msg/detail/byte_row__traits.hpp"

namespace main_interface
{

namespace msg
{

inline void to_flow_style_yaml(
  const ByteRows & msg,
  std::ostream & out)
{
  out << "{";
  // member: rows
  {
    if (msg.rows.size() == 0) {
      out << "rows: []";
    } else {
      out << "rows: [";
      size_t pending_items = msg.rows.size();
      for (auto item : msg.rows) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ByteRows & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: rows
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.rows.size() == 0) {
      out << "rows: []\n";
    } else {
      out << "rows:\n";
      for (auto item : msg.rows) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ByteRows & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace main_interface

namespace rosidl_generator_traits
{

[[deprecated("use main_interface::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const main_interface::msg::ByteRows & msg,
  std::ostream & out, size_t indentation = 0)
{
  main_interface::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use main_interface::msg::to_yaml() instead")]]
inline std::string to_yaml(const main_interface::msg::ByteRows & msg)
{
  return main_interface::msg::to_yaml(msg);
}

template<>
inline const char * data_type<main_interface::msg::ByteRows>()
{
  return "main_interface::msg::ByteRows";
}

template<>
inline const char * name<main_interface::msg::ByteRows>()
{
  return "main_interface/msg/ByteRows";
}

template<>
struct has_fixed_size<main_interface::msg::ByteRows>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<main_interface::msg::ByteRows>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<main_interface::msg::ByteRows>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__TRAITS_HPP_
