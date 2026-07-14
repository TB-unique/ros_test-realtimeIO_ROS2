#include "IOringbuffer/IOringbuffer.hpp"
#include "realtime_common/rt_utils.hpp"

#include <algorithm>
#include <cstring>

namespace io_ringbuffer
{
IOringbuffer::IOringbuffer(const rclcpp::NodeOptions & options) 
: rclcpp::Node("io_ringbuffer", options)
{
    // 声明参数
  declare_parameter("read_io_priority", 90);
  declare_parameter("write_io_priority", 90);
  declare_parameter("read_cpu_core", 2);
  declare_parameter("write_cpu_core", 2);
  declare_parameter("io_period_ms_read", 0.02);
  declare_parameter("io_period_ms_write", 0.02);
  
  get_parameter("read_io_priority", read_io_priority_);
  get_parameter("write_io_priority", write_io_priority_);
  get_parameter("read_cpu_core", read_cpu_core_);
  get_parameter("write_cpu_core", write_cpu_core_);
  get_parameter("io_period_ms_read", io_period_ms_read_);
  get_parameter("io_period_ms_write", io_period_ms_write_);

    // 创建 ROS 接口
    write_sub_ = this->create_subscription<main_interface::msg::ByteRow>(
        "input_topic", 10, 
        std::bind(&IOringbuffer::rosTopic_to_write_ringbuffer, this, std::placeholders::_1));

    read_pub_ = this->create_publisher<main_interface::msg::ByteRow>(
        "output_topic", 10);
    
    // 启动实时线程
    running_ = true;

    // 读 IO 线程（高优先级）
    read_io_thread_ = std::thread([this]() {
        realtime_common::set_cpu_affinity(read_cpu_core_);
        realtime_common::set_realtime_priority(read_io_priority_);
        read_io_thread();
    });

    read_ringbuffer_to_rosTopic_thread_ = std::thread([this]() {
        realtime_common::set_cpu_affinity(read_cpu_core_);
        realtime_common::set_realtime_priority(read_io_priority_-2);
        read_ringbuffer_to_rosTopic();
    });

    // 写 IO 线程（高优先级）
    write_io_thread_ = std::thread([this]() {
        realtime_common::set_cpu_affinity(write_cpu_core_);
        realtime_common::set_realtime_priority(write_io_priority_);
        write_io_thread();
    });

    RCLCPP_INFO(this->get_logger(), "IOringbuffer Component started");
    RCLCPP_INFO(this->get_logger(), "Read IO: CPU%d P%d, Period %.3fms",
                read_cpu_core_, read_io_priority_, io_period_ms_read_);
    RCLCPP_INFO(this->get_logger(), "Write IO: CPU%d P%d, Period %.3fms",
                write_cpu_core_, write_io_priority_, io_period_ms_write_);

}

IOringbuffer::~IOringbuffer()
{
    RCLCPP_INFO(this->get_logger(), "Shutting down IOringbuffer Component...");

    running_ = false;

    if (read_io_thread_.joinable()) {
        read_io_thread_.join();
    }

    if (read_ringbuffer_to_rosTopic_thread_.joinable()) {
        read_ringbuffer_to_rosTopic_thread_.join();
    }

    if (write_io_thread_.joinable()) {
        write_io_thread_.join();
    }

    RCLCPP_INFO(this->get_logger(), "Component stopped.");
}

/// === ROS 订阅回调（非实时，只做数据拷贝）===
void IOringbuffer::rosTopic_to_write_ringbuffer(
    const main_interface::msg::ByteRow::SharedPtr msg)
{
    RingBufferSlot slot;
    slot.size = std::min(msg->data.size(), MAX_MSG_SIZE);
    std::memcpy(slot.data.data(), msg->data.data(), slot.size);

    if (!write_ringbuffer_.try_push(slot)) {
        RCLCPP_WARN(this->get_logger(), "Write ringbuffer is full, discard message");
    }
}  
// === 读 IO 线程（实时）===
void IOringbuffer::read_io_thread()
{
    using clock = std::chrono::steady_clock;
    auto next_cycle = clock::now();
    const auto period = std::chrono::microseconds(static_cast<int64_t>(io_period_ms_read_ * 1000));
    
    main_interface::msg::ByteRow read_data;
    RingBufferSlot slot;
    while (running_) {
        auto start_time = clock::now();
        
        if (read_hardware(read_data)) {
            // ByteRow → RingBufferSlot
            slot.size = std::min(read_data.data.size(), MAX_MSG_SIZE);
            std::memcpy(slot.data.data(), read_data.data.data(), slot.size);

            if (!read_ringbuffer_.try_push(slot)) {
                RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                             "Read ringbuffer full, dropping data");
            }
        } else {
           RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                           "Hardware read failed");
        }
        // TODO: 实现读取IO的逻辑
        
        // 计算下一个周期时间
        next_cycle += period;
        std::this_thread::sleep_until(next_cycle);
    }
}

void IOringbuffer::read_ringbuffer_to_rosTopic()
{
    using clock = std::chrono::steady_clock;
    auto next_cycle = clock::now();
    const auto period = std::chrono::microseconds(static_cast<int64_t>(2 * 1000)); // 2ms 发布周期

    while (running_) {
        main_interface::msg::ByteRow ros_msg;
        RingBufferSlot slot;

        // 批量出队并拼接后发布
        while (read_ringbuffer_.try_pop(slot)) {
            ros_msg.data.insert(ros_msg.data.end(),
                                slot.data.begin(),
                                slot.data.begin() + slot.size);
        }

        if (!ros_msg.data.empty()) {
            read_pub_->publish(std::move(ros_msg));
        }

        // 计算下一个周期时间
        next_cycle += period;
        std::this_thread::sleep_until(next_cycle);
    }
}

void IOringbuffer::write_io_thread()
{
    using clock = std::chrono::steady_clock;
    auto next_cycle = clock::now();
    const auto period = std::chrono::microseconds(static_cast<int64_t>(io_period_ms_write_ * 1000));
    
    RingBufferSlot slot;
    while (running_) {
        // 从环形缓冲区读取数据
        if (write_ringbuffer_.try_pop(slot)) {
            // RingBufferSlot → ByteRow
            main_interface::msg::ByteRow write_data;
            write_data.data.assign(slot.data.begin(), slot.data.begin() + slot.size);

            if (!write_hardware(write_data)) {
                RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                             "Hardware write failed");
            }
        }

        // 计算下一个周期时间
        next_cycle += period;
        std::this_thread::sleep_until(next_cycle);
    }
}

bool IOringbuffer::read_hardware(main_interface::msg::ByteRow & data)
{
    // TODO: Implement hardware read logic
    // 返回 false 表示没有数据，避免空桩无限生产数据撑爆 ringbuffer
    return false;
}

bool IOringbuffer::write_hardware(const main_interface::msg::ByteRow & data)
{
    // TODO: Implement hardware write logic
    return true;
}

}  // namespace io_ringbuffer

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(io_ringbuffer::IOringbuffer)
