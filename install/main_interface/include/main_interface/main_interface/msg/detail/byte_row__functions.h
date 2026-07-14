// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from main_interface:msg/ByteRow.idl
// generated code does not contain a copyright notice

#ifndef MAIN_INTERFACE__MSG__DETAIL__BYTE_ROW__FUNCTIONS_H_
#define MAIN_INTERFACE__MSG__DETAIL__BYTE_ROW__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "main_interface/msg/rosidl_generator_c__visibility_control.h"

#include "main_interface/msg/detail/byte_row__struct.h"

/// Initialize msg/ByteRow message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * main_interface__msg__ByteRow
 * )) before or use
 * main_interface__msg__ByteRow__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
bool
main_interface__msg__ByteRow__init(main_interface__msg__ByteRow * msg);

/// Finalize msg/ByteRow message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
void
main_interface__msg__ByteRow__fini(main_interface__msg__ByteRow * msg);

/// Create msg/ByteRow message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * main_interface__msg__ByteRow__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
main_interface__msg__ByteRow *
main_interface__msg__ByteRow__create();

/// Destroy msg/ByteRow message.
/**
 * It calls
 * main_interface__msg__ByteRow__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
void
main_interface__msg__ByteRow__destroy(main_interface__msg__ByteRow * msg);

/// Check for msg/ByteRow message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
bool
main_interface__msg__ByteRow__are_equal(const main_interface__msg__ByteRow * lhs, const main_interface__msg__ByteRow * rhs);

/// Copy a msg/ByteRow message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
bool
main_interface__msg__ByteRow__copy(
  const main_interface__msg__ByteRow * input,
  main_interface__msg__ByteRow * output);

/// Initialize array of msg/ByteRow messages.
/**
 * It allocates the memory for the number of elements and calls
 * main_interface__msg__ByteRow__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
bool
main_interface__msg__ByteRow__Sequence__init(main_interface__msg__ByteRow__Sequence * array, size_t size);

/// Finalize array of msg/ByteRow messages.
/**
 * It calls
 * main_interface__msg__ByteRow__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
void
main_interface__msg__ByteRow__Sequence__fini(main_interface__msg__ByteRow__Sequence * array);

/// Create array of msg/ByteRow messages.
/**
 * It allocates the memory for the array and calls
 * main_interface__msg__ByteRow__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
main_interface__msg__ByteRow__Sequence *
main_interface__msg__ByteRow__Sequence__create(size_t size);

/// Destroy array of msg/ByteRow messages.
/**
 * It calls
 * main_interface__msg__ByteRow__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
void
main_interface__msg__ByteRow__Sequence__destroy(main_interface__msg__ByteRow__Sequence * array);

/// Check for msg/ByteRow message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
bool
main_interface__msg__ByteRow__Sequence__are_equal(const main_interface__msg__ByteRow__Sequence * lhs, const main_interface__msg__ByteRow__Sequence * rhs);

/// Copy an array of msg/ByteRow messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
bool
main_interface__msg__ByteRow__Sequence__copy(
  const main_interface__msg__ByteRow__Sequence * input,
  main_interface__msg__ByteRow__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // MAIN_INTERFACE__MSG__DETAIL__BYTE_ROW__FUNCTIONS_H_
