#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to main_interface__msg__ByteRow

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ByteRow {

    // This member is not documented.
    #[allow(missing_docs)]
    pub data: Vec<u8>,

}



impl Default for ByteRow {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::ByteRow::default())
  }
}

impl rosidl_runtime_rs::Message for ByteRow {
  type RmwMsg = super::msg::rmw::ByteRow;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        data: msg.data.into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        data: msg.data.as_slice().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      data: msg.data
          .into_iter()
          .collect(),
    }
  }
}


// Corresponds to main_interface__msg__ByteRows

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ByteRows {

    // This member is not documented.
    #[allow(missing_docs)]
    pub rows: Vec<super::msg::ByteRow>,

}



impl Default for ByteRows {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::ByteRows::default())
  }
}

impl rosidl_runtime_rs::Message for ByteRows {
  type RmwMsg = super::msg::rmw::ByteRows;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        rows: msg.rows
          .into_iter()
          .map(|elem| super::msg::ByteRow::into_rmw_message(std::borrow::Cow::Owned(elem)).into_owned())
          .collect(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        rows: msg.rows
          .iter()
          .map(|elem| super::msg::ByteRow::into_rmw_message(std::borrow::Cow::Borrowed(elem)).into_owned())
          .collect(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      rows: msg.rows
          .into_iter()
          .map(super::msg::ByteRow::from_rmw_message)
          .collect(),
    }
  }
}


