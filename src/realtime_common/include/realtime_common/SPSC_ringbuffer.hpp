#pragma once

#include <pthread.h>
#include <sched.h>
#include <cstring>
#include <iostream>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <new>
#include <utility>
#include <type_traits>

namespace realtime_common
{

constexpr std::size_t CACHE_LINE = 64;

/**
 * @brief 单生产者单消费者无锁环形缓冲区
 * @tparam T 元素类型（必须是 TriviallyCopyable）
 * @tparam Capacity 容量（必须是 2 的幂）
 *
 * ✅ 真正无锁
 * ✅ 零动态分配
 * ✅ 实时安全
 * ✅ 确定性延迟
 */
template<typename T, std::size_t Capacity>
class SPSCRingBuffer
{
  static_assert(std::is_trivially_copyable_v<T>,
                "SPSCRingBuffer requires trivially copyable type");
  static_assert((Capacity & (Capacity - 1)) == 0,
                "Capacity must be power of two");
  static_assert(Capacity >= 2, "Capacity must be at least 2");

public:
  SPSCRingBuffer() noexcept
    : read_idx_(0), write_idx_(0)
  {
    // 初始化所有 Slot（不调用构造函数）
    for (std::size_t i = 0; i < Capacity; ++i) {
      slots_[i].sequence.store(i, std::memory_order_relaxed);
    }
  }

  ~SPSCRingBuffer() noexcept
  {
    // 析构所有已写入但未消费的元素
    std::size_t read = read_idx_.load(std::memory_order_relaxed);
    std::size_t write = write_idx_.load(std::memory_order_relaxed);

    while (read != write) {
      slots_[read & mask()].data.~T();
      ++read;
    }
  }

  // 禁止拷贝（无锁结构不可浅拷贝）
  SPSCRingBuffer(const SPSCRingBuffer&) = delete;
  SPSCRingBuffer& operator=(const SPSCRingBuffer&) = delete;

  // ==================== 生产者接口 ====================

  /**
   * @brief 写入数据（拷贝）
   * @note 仅生产者线程调用
   */
  bool try_push(const T& value) noexcept
  {
    return try_emplace(value);
  }

  bool try_push(T&& value) noexcept
  {
    return try_emplace(std::move(value));
  }

  /**
   * @brief 原地构造（零拷贝）
   * @note 仅生产者线程调用
   */
  template<typename... Args>
  bool try_emplace(Args&&... args) noexcept
  {
    const std::size_t write = write_idx_.load(std::memory_order_relaxed);
    Slot& slot = slots_[write & mask()];

    // 检查槽位是否可用
    if (slot.sequence.load(std::memory_order_acquire) != write) {
      return false;  // 队列满
    }

    // 构造数据
    new (&slot.data) T(std::forward<Args>(args)...);

    // 发布数据（release 保证构造完成后再更新序列号）
    slot.sequence.store(write + 1, std::memory_order_release);

    // 推进写指针
    write_idx_.store(write + 1, std::memory_order_relaxed);
    return true;
  }

  // ==================== 消费者接口 ====================

  /**
   * @brief 读取并移除数据
   * @note 仅消费者线程调用
   */
  bool try_pop(T& out) noexcept
  {
    const std::size_t read = read_idx_.load(std::memory_order_relaxed);
    Slot& slot = slots_[read & mask()];

    // 检查是否有数据可读
    if (slot.sequence.load(std::memory_order_acquire) != read + 1) {
      return false;  // 队列空
    }

    // 读取数据
    out = std::move(slot.data);
    slot.data.~T();

    // 释放槽位（允许生产者再次使用）
    slot.sequence.store(read + Capacity, std::memory_order_release);

    // 推进读指针
    read_idx_.store(read + 1, std::memory_order_relaxed);
    return true;
  }

  /**
   * @brief 查看数据但不移除
   * @note 仅消费者线程调用
   */
  T* peek() noexcept
  {
    const std::size_t read = read_idx_.load(std::memory_order_relaxed);
    Slot& slot = slots_[read & mask()];

    if (slot.sequence.load(std::memory_order_acquire) != read + 1) {
      return nullptr;  // 队列空
    }

    return &slot.data;
  }

  /**
   * @brief 完成 peek 后的消费
   * @note 必须与 peek() 配对使用
   */
  void pop_finish() noexcept
  {
    const std::size_t read = read_idx_.load(std::memory_order_relaxed);
    Slot& slot = slots_[read & mask()];

    slot.data.~T();
    slot.sequence.store(read + Capacity, std::memory_order_release);
    read_idx_.store(read + 1, std::memory_order_relaxed);
  }

  // ==================== 批量操作（高性能关键）====================

  /**
   * @brief 批量消费
   * @return 实际消费的数量
   */
  template<typename OutputIterator>
  std::size_t pop_batch(OutputIterator out, std::size_t max_count) noexcept
  {
    std::size_t read = read_idx_.load(std::memory_order_relaxed);
    std::size_t available = available_read(read);

    if (available == 0) {
      return 0;
    }

    const std::size_t count = std::min(available, max_count);

    for (std::size_t i = 0; i < count; ++i) {
      Slot& slot = slots_[(read + i) & mask()];
      *out++ = std::move(slot.data);
      slot.data.~T();
      slot.sequence.store(read + i + Capacity, std::memory_order_release);
    }

    read_idx_.store(read + count, std::memory_order_relaxed);
    return count;
  }

  // ==================== 状态查询 ====================

  bool empty() const noexcept
  {
    return available_read() == 0;
  }

  bool full() const noexcept
  {
    return available_write() == 0;
  }

  /**
   * @brief 近似大小（非原子快照，仅用于监控）
   */
  std::size_t size_approx() const noexcept
  {
    return available_read();
  }

  constexpr std::size_t capacity() const noexcept
  {
    return Capacity;
  }

private:
  // 消除伪共享的 Slot
  struct alignas(CACHE_LINE) Slot
  {
    std::atomic<std::size_t> sequence;
    union {
      T data;
      char padding[sizeof(T) < CACHE_LINE ? CACHE_LINE : sizeof(T)];
    };

    Slot() {}
    ~Slot() {}
  };

  // 计算掩码（用于取模，比 % 快）
  constexpr std::size_t mask() const noexcept
  {
    return Capacity - 1;
  }

  std::size_t available_read() const noexcept
  {
    const std::size_t write = write_idx_.load(std::memory_order_acquire);
    const std::size_t read = read_idx_.load(std::memory_order_acquire);
    return write - read;
  }

  std::size_t available_read(std::size_t read) const noexcept
  {
    const std::size_t write = write_idx_.load(std::memory_order_acquire);
    return write - read;
  }

  std::size_t available_write() const noexcept
  {
    const std::size_t read = read_idx_.load(std::memory_order_acquire);
    const std::size_t write = write_idx_.load(std::memory_order_acquire);
    return Capacity - (write - read);
  }

private:
  // 读指针（单独 Cache Line）
  alignas(CACHE_LINE) std::atomic<std::size_t> read_idx_;

  // 写指针（单独 Cache Line）
  alignas(CACHE_LINE) std::atomic<std::size_t> write_idx_;

  // 环形缓冲区（单独 Cache Line）
  alignas(CACHE_LINE) Slot slots_[Capacity];
};

}  // namespace realtime_common