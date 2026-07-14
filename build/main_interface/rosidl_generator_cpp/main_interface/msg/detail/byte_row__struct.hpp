// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from main_interface:msg/ByteRow.idl
// generated code does not contain a copyright notice

#ifndef MAIN_INTERFACE__MSG__DETAIL__BYTE_ROW__STRUCT_HPP_
#define MAIN_INTERFACE__MSG__DETAIL__BYTE_ROW__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__main_interface__msg__ByteRow __attribute__((deprecated))
#else
# define DEPRECATED__main_interface__msg__ByteRow __declspec(deprecated)
#endif

namespace main_interface
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ByteRow_
{
  using Type = ByteRow_<ContainerAllocator>;

  explicit ByteRow_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
  }

  explicit ByteRow_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
    (void)_alloc;
  }

  // field types and members
  using _data_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _data_type data;

  // setters for named parameter idiom
  Type & set__data(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->data = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    main_interface::msg::ByteRow_<ContainerAllocator> *;
  using ConstRawPtr =
    const main_interface::msg::ByteRow_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<main_interface::msg::ByteRow_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<main_interface::msg::ByteRow_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      main_interface::msg::ByteRow_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<main_interface::msg::ByteRow_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      main_interface::msg::ByteRow_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<main_interface::msg::ByteRow_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<main_interface::msg::ByteRow_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<main_interface::msg::ByteRow_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__main_interface__msg__ByteRow
    std::shared_ptr<main_interface::msg::ByteRow_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__main_interface__msg__ByteRow
    std::shared_ptr<main_interface::msg::ByteRow_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ByteRow_ & other) const
  {
    if (this->data != other.data) {
      return false;
    }
    return true;
  }
  bool operator!=(const ByteRow_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ByteRow_

// alias to use template instance with default allocator
using ByteRow =
  main_interface::msg::ByteRow_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace main_interface

#endif  // MAIN_INTERFACE__MSG__DETAIL__BYTE_ROW__STRUCT_HPP_
