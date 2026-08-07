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

// Cache line size (x86_64)
constexpr std::size_t CACHE_LINE_SIZE = 64;

/**
 * @brief 高性能 SPSC 无锁队列（单生产者单消费者）
 * @tparam T 元素类型（必须是 trivially copyable）
 * @tparam Capacity 队列容量（必须是 2 的幂）
 */
template<typename T, std::size_t Capacity>
class SPSCQueue
{
  static_assert(std::is_trivially_copyable_v<T>,
                "SPSCQueue requires trivially copyable type");
  static_assert((Capacity & (Capacity - 1)) == 0,
                "Capacity must be power of two");
  static_assert(Capacity > 1, "Capacity must be at least 2");

public:
  SPSCQueue() noexcept
    : read_idx_(0), write_idx_(0)
  {
    // 预分配所有元素（避免运行时 new/delete）
    for (std::size_t i = 0; i < Capacity; ++i) {
      buffer_[i].storage.data.~T();  // 确保处于未初始化状态
    }
  }

  ~SPSCQueue() noexcept
  {
    // 销毁所有已构造的元素
    std::size_t read = read_idx_.load(std::memory_order_relaxed);
    std::size_t write = write_idx_.load(std::memory_order_relaxed);
    
    while (read != write) {
      buffer_[read & (Capacity - 1)].storage.data.~T();
      ++read;
    }
  }

  // 禁止拷贝（无锁结构不能浅拷贝）
  SPSCQueue(const SPSCQueue&) = delete;
  SPSCQueue& operator=(const SPSCQueue&) = delete;

  /**
   * @brief 入队（仅生产者线程调用）
   * @return true 成功, false 队列满
   */
  template<typename... Args>
  bool emplace(Args&&... args) noexcept
  {
    const std::size_t write = write_idx_.load(std::memory_order_relaxed);
    const std::size_t next_write = write + 1;
    
    // 关键：检查是否已满
    // 使用 acquire 确保读到最新的 read_idx_
    if (next_write == read_idx_.load(std::memory_order_acquire)) {
      return false;  // 队列满
    }
    
    // 构造元素（原地构造，无拷贝）
    new (&buffer_[write & (Capacity - 1)].storage.data) T(std::forward<Args>(args)...);
    
    // 关键：发布写入
    // release 确保构造完成后再更新 write_idx_
    write_idx_.store(next_write, std::memory_order_release);
    return true;
  }

  /**
   * @brief 入队（拷贝版本）
   */
  bool push(const T& value) noexcept
  {
    return emplace(value);
  }

  /**
   * @brief 入队（移动版本）
   */
  bool push(T&& value) noexcept
  {
    return emplace(std::move(value));
  }

  /**
   * @brief 出队（仅消费者线程调用）
   * @return true 成功, false 队列空
   */
  bool pop(T& out) noexcept
  {
    const std::size_t read = read_idx_.load(std::memory_order_relaxed);
    
    // 关键：检查是否为空
    // acquire 确保读到最新的 write_idx_
    if (read == write_idx_.load(std::memory_order_acquire)) {
      return false;  // 队列空
    }
    
    // 拷贝数据
    out = std::move(buffer_[read & (Capacity - 1)].storage.data);
    
    // 析构元素
    buffer_[read & (Capacity - 1)].storage.data.~T();
    
    // 关键：发布读取
    // release 确保析构完成后再更新 read_idx_
    read_idx_.store(read + 1, std::memory_order_release);
    return true;
  }

  /**
   * @brief 尝试出队（不拷贝，返回指针）
   * @note 必须在 pop_finish() 后调用
   */
  T* peek() noexcept
  {
    const std::size_t read = read_idx_.load(std::memory_order_relaxed);
    
    if (read == write_idx_.load(std::memory_order_acquire)) {
      return nullptr;
    }
    
    return &buffer_[read & (Capacity - 1)].storage.data;
  }

  /**
   * @brief 完成出队
   */
  void pop_finish() noexcept
  {
    const std::size_t read = read_idx_.load(std::memory_order_relaxed);
    buffer_[read & (Capacity - 1)].storage.data.~T();
    read_idx_.store(read + 1, std::memory_order_release);
  }

  /**
   * @brief 批量出队（减少内存序开销）
   */
  template<typename OutputIterator>
  std::size_t pop_bulk(OutputIterator out, std::size_t max_count) noexcept
  {
    std::size_t read = read_idx_.load(std::memory_order_relaxed);
    const std::size_t write = write_idx_.load(std::memory_order_acquire);
    
    if (read == write) {
      return 0;
    }
    
    const std::size_t available = write - read;
    const std::size_t count = std::min(available, max_count);
    
    for (std::size_t i = 0; i < count; ++i) {
      *out++ = std::move(buffer_[(read + i) & (Capacity - 1)].storage.data);
      buffer_[(read + i) & (Capacity - 1)].storage.data.~T();
    }
    
    read_idx_.store(read + count, std::memory_order_release);
    return count;
  }

  /**
   * @brief 判断是否为空
   */
  bool empty() const noexcept
  {
    return read_idx_.load(std::memory_order_acquire) ==
           write_idx_.load(std::memory_order_acquire);
  }

  /**
   * @brief 判断是否已满
   */
  bool full() const noexcept
  {
    return (write_idx_.load(std::memory_order_acquire)) -read_idx_.load(std::memory_order_acquire) 
            >= Capacity - 1;
  }

  /**
   * @brief 当前元素数量（近似值，非原子快照）
   */
  std::size_t size_approx() const noexcept
  {
    return
        write_idx_.load(std::memory_order_acquire) -
        read_idx_.load(std::memory_order_acquire);
  }

  /**
   * @brief 容量
   */
  constexpr std::size_t capacity() const noexcept { return Capacity; }

private:
  // 防止 false sharing 的关键结构
  struct alignas(CACHE_LINE_SIZE) Slot
  {
    union Storage
    {
      T data;
      char padding[sizeof(T) < CACHE_LINE_SIZE ? CACHE_LINE_SIZE : sizeof(T)];
      
      Storage() {}
      ~Storage() {}
    } storage;
  };

  // 读指针（单独 cache line）
  alignas(CACHE_LINE_SIZE) std::atomic<std::size_t> read_idx_;

  // 写指针（单独 cache line）
  alignas(CACHE_LINE_SIZE) std::atomic<std::size_t> write_idx_;

  // 环形缓冲区
  alignas(CACHE_LINE_SIZE) Slot buffer_[Capacity];
};

// 等待策略
enum class WaitStrategy
{
  SPIN_ONLY,      // 只忙等（< 10μs）
  SLEEP_ONLY,     // 只休眠（≥ 100μs）
  HYBRID          // 先忙等后休眠（推荐）
};

//-------------------超时等待的 SPSC 队列（可选）-------------------------------------

template<typename T, std::size_t Capacity>
class SPSCQueueTimeout : public SPSCQueue<T, Capacity>
{
public:
  explicit SPSCQueueTimeout(
    WaitStrategy strategy = WaitStrategy::HYBRID,
    uint64_t spin_cycles = 1000) noexcept
    : strategy_(strategy),
      spin_cycles_(spin_cycles)
  {}

  // ===== 带超时的 Push =====
  
  template<typename... Args>
  bool try_emplace(uint64_t timeout_ns, Args&&... args) noexcept
  {
    const auto start = std::chrono::steady_clock::now();
    uint64_t elapsed = 0;
    
    while (elapsed < timeout_ns) {
      if (this->emplace(std::forward<Args>(args)...)) {
        return true;
      }
      
      if (!wait_once(elapsed, timeout_ns, start)) {
        break;
      }
    }
    return false;
  }

  bool try_push(const T& value, uint64_t timeout_ns) noexcept
  {
    return try_emplace(timeout_ns, value);
  }

  bool try_push(T&& value, uint64_t timeout_ns) noexcept
  {
    return try_emplace(timeout_ns, std::move(value));
  }

  // ===== 带超时的 Pop =====
  
  bool try_pop(T& out, uint64_t timeout_ns) noexcept
  {
    const auto start = std::chrono::steady_clock::now();
    uint64_t elapsed = 0;
    
    while (elapsed < timeout_ns) {
      if (this->pop(out)) {
        return true;
      }
      
      if (!wait_once(elapsed, timeout_ns, start)) {
        break;
      }
    }
    return false;
  }

  T* try_peek(uint64_t timeout_ns) noexcept
  {
    const auto start = std::chrono::steady_clock::now();
    uint64_t elapsed = 0;
    
    while (elapsed < timeout_ns) {
      T* ptr = this->peek();
      if (ptr) {
        return ptr;
      }
      
      if (!wait_once(elapsed, timeout_ns, start)) {
        break;
      }
    }
    return nullptr;
  }

private:
  // 单次等待（核心逻辑）
  template<typename Clock, typename Duration>
  bool wait_once(
    uint64_t& elapsed,
    uint64_t timeout_ns,
    const std::chrono::time_point<Clock, Duration>& start) noexcept
  {
    switch (strategy_) {
      case WaitStrategy::SPIN_ONLY:
        return spin_wait(elapsed, timeout_ns, start);
      case WaitStrategy::SLEEP_ONLY:
        return sleep_wait(elapsed, timeout_ns, start);
      case WaitStrategy::HYBRID:
        return hybrid_wait(elapsed, timeout_ns, start);
      default:
        return false;
    }
  }

  // 忙等（精确但耗 CPU）
  template<typename Clock, typename Duration>
  bool spin_wait(
    uint64_t& elapsed,
    uint64_t timeout_ns,
    const std::chrono::time_point<Clock, Duration>& start) noexcept
  {
    // 简单的 CPU 空转
    for (uint64_t i = 0; i < spin_cycles_; ++i) {
      asm volatile("nop");  // 防止编译器优化掉循环
    }
    
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
      Clock::now() - start).count();
    return elapsed < timeout_ns;
  }

  // 休眠（省 CPU 但精度低）
  template<typename Clock, typename Duration>
  bool sleep_wait(
    uint64_t& elapsed,
    uint64_t timeout_ns,
    const std::chrono::time_point<Clock, Duration>& start) noexcept
  {
    // 计算剩余时间
    auto remaining_ns = timeout_ns - elapsed;
    if (remaining_ns <= 0) return false;
    
    // 最小休眠单位：100ns（避免过于频繁的系统调用）
    auto sleep_duration = std::chrono::nanoseconds(
      std::max<uint64_t>(remaining_ns / 10, 100));
    
    std::this_thread::sleep_for(sleep_duration);
    
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
      Clock::now() - start).count();
    return elapsed < timeout_ns;
  }

  // 混合策略（推荐）
  template<typename Clock, typename Duration>
  bool hybrid_wait(
    uint64_t& elapsed,
    uint64_t timeout_ns,
    const std::chrono::time_point<Clock, Duration>& start) noexcept
  {
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
      Clock::now() - start).count();
    
    if (elapsed >= timeout_ns) {
      return false;
    }
    
    auto remaining_ns = timeout_ns - elapsed;
    
    // < 50μs：忙等
    if (remaining_ns < 50'000) {
      return spin_wait(elapsed, timeout_ns, start);
    }
    
    // ≥ 50μs：休眠（剩余时间的 1/4）
    auto sleep_ns = remaining_ns / 4;
    if (sleep_ns > 1'000'000) sleep_ns = 1'000'000;  // 最多 1ms
    
    std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_ns));
    
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
      Clock::now() - start).count();
    return elapsed < timeout_ns;
  }

private:
  WaitStrategy strategy_;
  uint64_t spin_cycles_;  // 忙等循环次数
};

/**
 * @brief 阻塞式 SPSC 队列（继承 SPSCQueue）
 *
 * ✅ Producer：完全复用 SPSCQueue，零锁、零阻塞
 * ✅ Consumer：新增阻塞等待能力
 *
 * 典型用途：
 *   - RT 线程 → ROS / UI / Log 线程
 */
template<typename T, std::size_t Capacity>
class BlockingSPSCQueue final
  : private SPSCQueue<T, Capacity>
{
public:
  using Base = SPSCQueue<T, Capacity>;

  BlockingSPSCQueue() noexcept = default;

  ~BlockingSPSCQueue() noexcept
  {
    stop();
  }

  // ==================== Producer API（无锁 + 条件通知）====================

  /**
   * @brief 入队（非阻塞），成功后通知阻塞的消费者
   * @return true 成功，false 队列满（主动丢数）
   *
   * ✅ 可在硬实时线程中调用（notify_one 无需持锁）
   */
  bool push(const T& value) noexcept
  {
    bool ok = Base::push(value);
    if (ok) {
      cv_.notify_one();
    }
    return ok;
  }

  template<typename... Args>
  bool emplace(Args&&... args) noexcept
  {
    bool ok = Base::emplace(std::forward<Args>(args)...);
    if (ok) {
      cv_.notify_one();
    }
    return ok;
  }

  // ==================== Consumer API（阻塞）====================

  /**
   * @brief 出队（阻塞）
   * @return true 成功取出数据, false 队列已停止
   *
   * ❌ 禁止在 RT 线程中调用
   */
  bool pop(T& out)
  {
    std::unique_lock<std::mutex> lock(mutex_);

    cv_.wait(lock, [this]() {
      return stopped_.load(std::memory_order_relaxed) ||
             !Base::empty();
    });

    if (stopped_) {
      return false;
    }

    Base::pop(out);
    return true;
  }

  /**
   * @brief 非阻塞出队（委托基类 SPSCQueue::pop）
   */
  bool try_pop(T& out) noexcept
  {
    return Base::pop(out);
  }

  /**
   * @brief 批量消费（强烈推荐）
   */
  template<typename OutputIterator>
  std::size_t pop_batch(OutputIterator out, std::size_t max_count)
  {
    std::unique_lock<std::mutex> lock(mutex_);

    cv_.wait(lock, [this]() {
      return stopped_.load(std::memory_order_relaxed) ||
             !Base::empty();
    });

    if (stopped_) {
      return 0;
    }

    return Base::pop_bulk(out, max_count);
  }

  // ==================== 状态查询（继承）====================

  using Base::empty;
  using Base::full;
  using Base::size_approx;
  using Base::capacity;

  // ==================== 生命周期 ====================

  void stop() noexcept
  {
    stopped_.store(true, std::memory_order_relaxed);
    cv_.notify_all();
  }

private:
  // ✅ Consumer-only 同步原语
  // 与 SPSCQueue 内部原子变量物理隔离
  alignas(64) std::mutex mutex_;
  std::condition_variable cv_;
  std::atomic<bool> stopped_{false};
};


}  // namespace realtime_common