// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from main_interface:msg/ByteRows.idl
// generated code does not contain a copyright notice

#ifndef MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__FUNCTIONS_H_
#define MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "main_interface/msg/rosidl_generator_c__visibility_control.h"

#include "main_interface/msg/detail/byte_rows__struct.h"

/// Initialize msg/ByteRows message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * main_interface__msg__ByteRows
 * )) before or use
 * main_interface__msg__ByteRows__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
bool
main_interface__msg__ByteRows__init(main_interface__msg__ByteRows * msg);

/// Finalize msg/ByteRows message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
void
main_interface__msg__ByteRows__fini(main_interface__msg__ByteRows * msg);

/// Create msg/ByteRows message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * main_interface__msg__ByteRows__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
main_interface__msg__ByteRows *
main_interface__msg__ByteRows__create();

/// Destroy msg/ByteRows message.
/**
 * It calls
 * main_interface__msg__ByteRows__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
void
main_interface__msg__ByteRows__destroy(main_interface__msg__ByteRows * msg);

/// Check for msg/ByteRows message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
bool
main_interface__msg__ByteRows__are_equal(const main_interface__msg__ByteRows * lhs, const main_interface__msg__ByteRows * rhs);

/// Copy a msg/ByteRows message.
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
main_interface__msg__ByteRows__copy(
  const main_interface__msg__ByteRows * input,
  main_interface__msg__ByteRows * output);

/// Initialize array of msg/ByteRows messages.
/**
 * It allocates the memory for the number of elements and calls
 * main_interface__msg__ByteRows__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
bool
main_interface__msg__ByteRows__Sequence__init(main_interface__msg__ByteRows__Sequence * array, size_t size);

/// Finalize array of msg/ByteRows messages.
/**
 * It calls
 * main_interface__msg__ByteRows__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
void
main_interface__msg__ByteRows__Sequence__fini(main_interface__msg__ByteRows__Sequence * array);

/// Create array of msg/ByteRows messages.
/**
 * It allocates the memory for the array and calls
 * main_interface__msg__ByteRows__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
main_interface__msg__ByteRows__Sequence *
main_interface__msg__ByteRows__Sequence__create(size_t size);

/// Destroy array of msg/ByteRows messages.
/**
 * It calls
 * main_interface__msg__ByteRows__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
void
main_interface__msg__ByteRows__Sequence__destroy(main_interface__msg__ByteRows__Sequence * array);

/// Check for msg/ByteRows message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_main_interface
bool
main_interface__msg__ByteRows__Sequence__are_equal(const main_interface__msg__ByteRows__Sequence * lhs, const main_interface__msg__ByteRows__Sequence * rhs);

/// Copy an array of msg/ByteRows messages.
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
main_interface__msg__ByteRows__Sequence__copy(
  const main_interface__msg__ByteRows__Sequence * input,
  main_interface__msg__ByteRows__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__FUNCTIONS_H_
