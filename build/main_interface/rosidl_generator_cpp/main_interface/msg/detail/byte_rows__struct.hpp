// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from main_interface:msg/ByteRows.idl
// generated code does not contain a copyright notice

#ifndef MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__STRUCT_HPP_
#define MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'rows'
#include "main_interface/msg/detail/byte_row__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__main_interface__msg__ByteRows __attribute__((deprecated))
#else
# define DEPRECATED__main_interface__msg__ByteRows __declspec(deprecated)
#endif

namespace main_interface
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ByteRows_
{
  using Type = ByteRows_<ContainerAllocator>;

  explicit ByteRows_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
  }

  explicit ByteRows_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
    (void)_alloc;
  }

  // field types and members
  using _rows_type =
    std::vector<main_interface::msg::ByteRow_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<main_interface::msg::ByteRow_<ContainerAllocator>>>;
  _rows_type rows;

  // setters for named parameter idiom
  Type & set__rows(
    const std::vector<main_interface::msg::ByteRow_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<main_interface::msg::ByteRow_<ContainerAllocator>>> & _arg)
  {
    this->rows = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    main_interface::msg::ByteRows_<ContainerAllocator> *;
  using ConstRawPtr =
    const main_interface::msg::ByteRows_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<main_interface::msg::ByteRows_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<main_interface::msg::ByteRows_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      main_interface::msg::ByteRows_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<main_interface::msg::ByteRows_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      main_interface::msg::ByteRows_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<main_interface::msg::ByteRows_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<main_interface::msg::ByteRows_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<main_interface::msg::ByteRows_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__main_interface__msg__ByteRows
    std::shared_ptr<main_interface::msg::ByteRows_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__main_interface__msg__ByteRows
    std::shared_ptr<main_interface::msg::ByteRows_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ByteRows_ & other) const
  {
    if (this->rows != other.rows) {
      return false;
    }
    return true;
  }
  bool operator!=(const ByteRows_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ByteRows_

// alias to use template instance with default allocator
using ByteRows =
  main_interface::msg::ByteRows_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace main_interface

#endif  // MAIN_INTERFACE__MSG__DETAIL__BYTE_ROWS__STRUCT_HPP_
