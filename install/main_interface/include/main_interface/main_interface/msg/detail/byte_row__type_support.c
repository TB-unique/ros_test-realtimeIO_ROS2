// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from main_interface:msg/ByteRow.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "main_interface/msg/detail/byte_row__rosidl_typesupport_introspection_c.h"
#include "main_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "main_interface/msg/detail/byte_row__functions.h"
#include "main_interface/msg/detail/byte_row__struct.h"


// Include directives for member types
// Member `data`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__ByteRow_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  main_interface__msg__ByteRow__init(message_memory);
}

void main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__ByteRow_fini_function(void * message_memory)
{
  main_interface__msg__ByteRow__fini(message_memory);
}

size_t main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__size_function__ByteRow__data(
  const void * untyped_member)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return member->size;
}

const void * main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__get_const_function__ByteRow__data(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__uint8__Sequence * member =
    (const rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void * main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__get_function__ByteRow__data(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  return &member->data[index];
}

void main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__fetch_function__ByteRow__data(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__get_const_function__ByteRow__data(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__assign_function__ByteRow__data(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__get_function__ByteRow__data(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__resize_function__ByteRow__data(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__uint8__Sequence * member =
    (rosidl_runtime_c__uint8__Sequence *)(untyped_member);
  rosidl_runtime_c__uint8__Sequence__fini(member);
  return rosidl_runtime_c__uint8__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__ByteRow_message_member_array[1] = {
  {
    "data",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(main_interface__msg__ByteRow, data),  // bytes offset in struct
    NULL,  // default value
    main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__size_function__ByteRow__data,  // size() function pointer
    main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__get_const_function__ByteRow__data,  // get_const(index) function pointer
    main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__get_function__ByteRow__data,  // get(index) function pointer
    main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__fetch_function__ByteRow__data,  // fetch(index, &value) function pointer
    main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__assign_function__ByteRow__data,  // assign(index, value) function pointer
    main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__resize_function__ByteRow__data  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__ByteRow_message_members = {
  "main_interface__msg",  // message namespace
  "ByteRow",  // message name
  1,  // number of fields
  sizeof(main_interface__msg__ByteRow),
  main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__ByteRow_message_member_array,  // message members
  main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__ByteRow_init_function,  // function to initialize message memory (memory has to be allocated)
  main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__ByteRow_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__ByteRow_message_type_support_handle = {
  0,
  &main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__ByteRow_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_main_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, main_interface, msg, ByteRow)() {
  if (!main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__ByteRow_message_type_support_handle.typesupport_identifier) {
    main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__ByteRow_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &main_interface__msg__ByteRow__rosidl_typesupport_introspection_c__ByteRow_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
