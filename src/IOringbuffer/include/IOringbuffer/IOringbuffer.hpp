#pragma once

#include <array>
#include <rclcpp/rclcpp.hpp>
#include "main_interface/msg/byte_row.hpp"
#include "realtime_common/SPSC_ringbuffer.hpp"

namespace io_ringbuffer
{

// 单条消息最大字节数（需根据实际需求调整）
constexpr std::size_t MAX_MSG_SIZE = 1024;

// 用于 ringbuffer 的固定大小存储单元（trivially copyable）
struct alignas(64) RingBufferSlot
{
  std::array<uint8_t, MAX_MSG_SIZE> data{};
  std::size_t size{0};  // 实际有效字节数
};
class IOringbuffer : public rclcpp::Node
{
public:
  explicit IOringbuffer(const rclcpp::NodeOptions & options);
  virtual ~IOringbuffer();

private:
    // === 实时线程函数 ===
    void read_io_thread();
    void write_io_thread();

    // === 硬件 IO 接口（需要你实现）===
    bool read_hardware(main_interface::msg::ByteRow & data);
    bool write_hardware(const main_interface::msg::ByteRow & data);

    void read_ringbuffer_to_rosTopic();
    void rosTopic_to_write_ringbuffer(
        const main_interface::msg::ByteRow::SharedPtr msg);

    // === 实时线程 ===
    std::thread read_io_thread_;
    std::thread read_ringbuffer_to_rosTopic_thread_;
    std::thread write_io_thread_;
    std::atomic<bool> running_{false};

    // === ROS 接口 ===
    rclcpp::Subscription<main_interface::msg::ByteRow>::SharedPtr write_sub_;
    rclcpp::Publisher<main_interface::msg::ByteRow>::SharedPtr read_pub_;

    // === 无锁环形缓冲区 ===
    static constexpr std::size_t READ_RINGBUFFER_CAP = 256;   // 2的幂
    static constexpr std::size_t WRITE_RINGBUFFER_CAP = 128;  // 2的幂
    realtime_common::SPSCRingBuffer<RingBufferSlot, READ_RINGBUFFER_CAP> read_ringbuffer_;    // IO → ROS
    realtime_common::SPSCRingBuffer<RingBufferSlot, WRITE_RINGBUFFER_CAP> write_ringbuffer_;  // ROS → IO

    // === 参数 ===
    int read_io_priority_{90};
    int write_io_priority_{90};
    int read_cpu_core_{2};
    int write_cpu_core_{2};
    double io_period_ms_read_{0.02};
    double io_period_ms_write_{0.02};
};
}  