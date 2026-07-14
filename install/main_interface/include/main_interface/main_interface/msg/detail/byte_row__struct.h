// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from main_interface:msg/ByteRow.idl
// generated code does not contain a copyright notice

#ifndef MAIN_INTERFACE__MSG__DETAIL__BYTE_ROW__STRUCT_H_
#define MAIN_INTERFACE__MSG__DETAIL__BYTE_ROW__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'data'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/ByteRow in the package main_interface.
typedef struct main_interface__msg__ByteRow
{
  rosidl_runtime_c__uint8__Sequence data;
} main_interface__msg__ByteRow;

// Struct for a sequence of main_interface__msg__ByteRow.
typedef struct main_interface__msg__ByteRow__Sequence
{
  main_interface__msg__ByteRow * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} main_interface__msg__ByteRow__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MAIN_INTERFACE__MSG__DETAIL__BYTE_ROW__STRUCT_H_
