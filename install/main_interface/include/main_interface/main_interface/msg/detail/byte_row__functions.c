// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from main_interface:msg/ByteRow.idl
// generated code does not contain a copyright notice
#include "main_interface/msg/detail/byte_row__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `data`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
main_interface__msg__ByteRow__init(main_interface__msg__ByteRow * msg)
{
  if (!msg) {
    return false;
  }
  // data
  if (!rosidl_runtime_c__uint8__Sequence__init(&msg->data, 0)) {
    main_interface__msg__ByteRow__fini(msg);
    return false;
  }
  return true;
}

void
main_interface__msg__ByteRow__fini(main_interface__msg__ByteRow * msg)
{
  if (!msg) {
    return;
  }
  // data
  rosidl_runtime_c__uint8__Sequence__fini(&msg->data);
}

bool
main_interface__msg__ByteRow__are_equal(const main_interface__msg__ByteRow * lhs, const main_interface__msg__ByteRow * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // data
  if (!rosidl_runtime_c__uint8__Sequence__are_equal(
      &(lhs->data), &(rhs->data)))
  {
    return false;
  }
  return true;
}

bool
main_interface__msg__ByteRow__copy(
  const main_interface__msg__ByteRow * input,
  main_interface__msg__ByteRow * output)
{
  if (!input || !output) {
    return false;
  }
  // data
  if (!rosidl_runtime_c__uint8__Sequence__copy(
      &(input->data), &(output->data)))
  {
    return false;
  }
  return true;
}

main_interface__msg__ByteRow *
main_interface__msg__ByteRow__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  main_interface__msg__ByteRow * msg = (main_interface__msg__ByteRow *)allocator.allocate(sizeof(main_interface__msg__ByteRow), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(main_interface__msg__ByteRow));
  bool success = main_interface__msg__ByteRow__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
main_interface__msg__ByteRow__destroy(main_interface__msg__ByteRow * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    main_interface__msg__ByteRow__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
main_interface__msg__ByteRow__Sequence__init(main_interface__msg__ByteRow__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  main_interface__msg__ByteRow * data = NULL;

  if (size) {
    data = (main_interface__msg__ByteRow *)allocator.zero_allocate(size, sizeof(main_interface__msg__ByteRow), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = main_interface__msg__ByteRow__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        main_interface__msg__ByteRow__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
main_interface__msg__ByteRow__Sequence__fini(main_interface__msg__ByteRow__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      main_interface__msg__ByteRow__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

main_interface__msg__ByteRow__Sequence *
main_interface__msg__ByteRow__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  main_interface__msg__ByteRow__Sequence * array = (main_interface__msg__ByteRow__Sequence *)allocator.allocate(sizeof(main_interface__msg__ByteRow__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = main_interface__msg__ByteRow__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
main_interface__msg__ByteRow__Sequence__destroy(main_interface__msg__ByteRow__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    main_interface__msg__ByteRow__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
main_interface__msg__ByteRow__Sequence__are_equal(const main_interface__msg__ByteRow__Sequence * lhs, const main_interface__msg__ByteRow__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!main_interface__msg__ByteRow__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
main_interface__msg__ByteRow__Sequence__copy(
  const main_interface__msg__ByteRow__Sequence * input,
  main_interface__msg__ByteRow__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(main_interface__msg__ByteRow);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    main_interface__msg__ByteRow * data =
      (main_interface__msg__ByteRow *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!main_interface__msg__ByteRow__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          main_interface__msg__ByteRow__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!main_interface__msg__ByteRow__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
