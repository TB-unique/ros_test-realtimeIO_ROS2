// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from main_interface:msg/ByteRows.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "main_interface/msg/detail/byte_rows__rosidl_typesupport_introspection_c.h"
#include "main_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "main_interface/msg/detail/byte_rows__functions.h"
#include "main_interface/msg/detail/byte_rows__struct.h"


// Include directives for member types
// Member `rows`
#include "main_interface/msg/byte_row.h"
// Member `rows`
#include "main_interface/msg/detail/byte_row__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  main_interface__msg__ByteRows__init(message_memory);
}

void main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_fini_function(void * message_memory)
{
  main_interface__msg__ByteRows__fini(message_memory);
}

size_t main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__size_function__ByteRows__rows(
  const void * untyped_member)
{
  const main_interface__msg__ByteRow__Sequence * member =
    (const main_interface__msg__ByteRow__Sequence *)(untyped_member);
  return member->size;
}

const void * main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__get_const_function__ByteRows__rows(
  const void * untyped_member, size_t index)
{
  const main_interface__msg__ByteRow__Sequence * member =
    (const main_interface__msg__ByteRow__Sequence *)(untyped_member);
  return &member->data[index];
}

void * main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__get_function__ByteRows__rows(
  void * untyped_member, size_t index)
{
  main_interface__msg__ByteRow__Sequence * member =
    (main_interface__msg__ByteRow__Sequence *)(untyped_member);
  return &member->data[index];
}

void main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__fetch_function__ByteRows__rows(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const main_interface__msg__ByteRow * item =
    ((const main_interface__msg__ByteRow *)
    main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__get_const_function__ByteRows__rows(untyped_member, index));
  main_interface__msg__ByteRow * value =
    (main_interface__msg__ByteRow *)(untyped_value);
  *value = *item;
}

void main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__assign_function__ByteRows__rows(
  void * untyped_member, size_t index, const void * untyped_value)
{
  main_interface__msg__ByteRow * item =
    ((main_interface__msg__ByteRow *)
    main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__get_function__ByteRows__rows(untyped_member, index));
  const main_interface__msg__ByteRow * value =
    (const main_interface__msg__ByteRow *)(untyped_value);
  *item = *value;
}

bool main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__resize_function__ByteRows__rows(
  void * untyped_member, size_t size)
{
  main_interface__msg__ByteRow__Sequence * member =
    (main_interface__msg__ByteRow__Sequence *)(untyped_member);
  main_interface__msg__ByteRow__Sequence__fini(member);
  return main_interface__msg__ByteRow__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_message_member_array[1] = {
  {
    "rows",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(main_interface__msg__ByteRows, rows),  // bytes offset in struct
    NULL,  // default value
    main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__size_function__ByteRows__rows,  // size() function pointer
    main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__get_const_function__ByteRows__rows,  // get_const(index) function pointer
    main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__get_function__ByteRows__rows,  // get(index) function pointer
    main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__fetch_function__ByteRows__rows,  // fetch(index, &value) function pointer
    main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__assign_function__ByteRows__rows,  // assign(index, value) function pointer
    main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__resize_function__ByteRows__rows  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_message_members = {
  "main_interface__msg",  // message namespace
  "ByteRows",  // message name
  1,  // number of fields
  sizeof(main_interface__msg__ByteRows),
  main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_message_member_array,  // message members
  main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_init_function,  // function to initialize message memory (memory has to be allocated)
  main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_message_type_support_handle = {
  0,
  &main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_main_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, main_interface, msg, ByteRows)() {
  main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, main_interface, msg, ByteRow)();
  if (!main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_message_type_support_handle.typesupport_identifier) {
    main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &main_interface__msg__ByteRows__rosidl_typesupport_introspection_c__ByteRows_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
