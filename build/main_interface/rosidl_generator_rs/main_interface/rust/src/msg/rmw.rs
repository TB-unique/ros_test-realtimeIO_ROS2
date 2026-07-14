#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "main_interface__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__main_interface__msg__ByteRow() -> *const std::ffi::c_void;
}

#[link(name = "main_interface__rosidl_generator_c")]
extern "C" {
    fn main_interface__msg__ByteRow__init(msg: *mut ByteRow) -> bool;
    fn main_interface__msg__ByteRow__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ByteRow>, size: usize) -> bool;
    fn main_interface__msg__ByteRow__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ByteRow>);
    fn main_interface__msg__ByteRow__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ByteRow>, out_seq: *mut rosidl_runtime_rs::Sequence<ByteRow>) -> bool;
}

// Corresponds to main_interface__msg__ByteRow
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ByteRow {

    // This member is not documented.
    #[allow(missing_docs)]
    pub data: rosidl_runtime_rs::Sequence<u8>,

}



impl Default for ByteRow {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !main_interface__msg__ByteRow__init(&mut msg as *mut _) {
        panic!("Call to main_interface__msg__ByteRow__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ByteRow {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { main_interface__msg__ByteRow__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { main_interface__msg__ByteRow__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { main_interface__msg__ByteRow__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ByteRow {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ByteRow where Self: Sized {
  const TYPE_NAME: &'static str = "main_interface/msg/ByteRow";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__main_interface__msg__ByteRow() }
  }
}


#[link(name = "main_interface__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__main_interface__msg__ByteRows() -> *const std::ffi::c_void;
}

#[link(name = "main_interface__rosidl_generator_c")]
extern "C" {
    fn main_interface__msg__ByteRows__init(msg: *mut ByteRows) -> bool;
    fn main_interface__msg__ByteRows__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ByteRows>, size: usize) -> bool;
    fn main_interface__msg__ByteRows__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ByteRows>);
    fn main_interface__msg__ByteRows__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ByteRows>, out_seq: *mut rosidl_runtime_rs::Sequence<ByteRows>) -> bool;
}

// Corresponds to main_interface__msg__ByteRows
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ByteRows {

    // This member is not documented.
    #[allow(missing_docs)]
    pub rows: rosidl_runtime_rs::Sequence<super::super::msg::rmw::ByteRow>,

}



impl Default for ByteRows {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !main_interface__msg__ByteRows__init(&mut msg as *mut _) {
        panic!("Call to main_interface__msg__ByteRows__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ByteRows {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { main_interface__msg__ByteRows__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { main_interface__msg__ByteRows__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { main_interface__msg__ByteRows__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ByteRows {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ByteRows where Self: Sized {
  const TYPE_NAME: &'static str = "main_interface/msg/ByteRows";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__main_interface__msg__ByteRows() }
  }
}


