// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from main_interface:msg/ByteRows.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "main_interface/msg/detail/byte_rows__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace main_interface
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void ByteRows_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) main_interface::msg::ByteRows(_init);
}

void ByteRows_fini_function(void * message_memory)
{
  auto typed_message = static_cast<main_interface::msg::ByteRows *>(message_memory);
  typed_message->~ByteRows();
}

size_t size_function__ByteRows__rows(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<main_interface::msg::ByteRow> *>(untyped_member);
  return member->size();
}

const void * get_const_function__ByteRows__rows(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<main_interface::msg::ByteRow> *>(untyped_member);
  return &member[index];
}

void * get_function__ByteRows__rows(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<main_interface::msg::ByteRow> *>(untyped_member);
  return &member[index];
}

void fetch_function__ByteRows__rows(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const main_interface::msg::ByteRow *>(
    get_const_function__ByteRows__rows(untyped_member, index));
  auto & value = *reinterpret_cast<main_interface::msg::ByteRow *>(untyped_value);
  value = item;
}

void assign_function__ByteRows__rows(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<main_interface::msg::ByteRow *>(
    get_function__ByteRows__rows(untyped_member, index));
  const auto & value = *reinterpret_cast<const main_interface::msg::ByteRow *>(untyped_value);
  item = value;
}

void resize_function__ByteRows__rows(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<main_interface::msg::ByteRow> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember ByteRows_message_member_array[1] = {
  {
    "rows",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<main_interface::msg::ByteRow>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(main_interface::msg::ByteRows, rows),  // bytes offset in struct
    nullptr,  // default value
    size_function__ByteRows__rows,  // size() function pointer
    get_const_function__ByteRows__rows,  // get_const(index) function pointer
    get_function__ByteRows__rows,  // get(index) function pointer
    fetch_function__ByteRows__rows,  // fetch(index, &value) function pointer
    assign_function__ByteRows__rows,  // assign(index, value) function pointer
    resize_function__ByteRows__rows  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers ByteRows_message_members = {
  "main_interface::msg",  // message namespace
  "ByteRows",  // message name
  1,  // number of fields
  sizeof(main_interface::msg::ByteRows),
  ByteRows_message_member_array,  // message members
  ByteRows_init_function,  // function to initialize message memory (memory has to be allocated)
  ByteRows_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t ByteRows_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &ByteRows_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace main_interface


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<main_interface::msg::ByteRows>()
{
  return &::main_interface::msg::rosidl_typesupport_introspection_cpp::ByteRows_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, main_interface, msg, ByteRows)() {
  return &::main_interface::msg::rosidl_typesupport_introspection_cpp::ByteRows_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
