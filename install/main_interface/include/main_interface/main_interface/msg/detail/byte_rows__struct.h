// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from main_interface:msg/ByteRows.idl
// generated code does not contain a copyright notice

#ifndef MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__STRUCT_H_
#define MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'rows'
#include "main_interface/msg/detail/byte_row__struct.h"

/// Struct defined in msg/ByteRows in the package main_interface.
typedef struct main_interface__msg__ByteRows
{
  main_interface__msg__ByteRow__Sequence rows;
} main_interface__msg__ByteRows;

// Struct for a sequence of main_interface__msg__ByteRows.
typedef struct main_interface__msg__ByteRows__Sequence
{
  main_interface__msg__ByteRows * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} main_interface__msg__ByteRows__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__STRUCT_H_
