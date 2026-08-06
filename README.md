# ros_test — ROS 2 实时 IO 环回测试框架

验证 **实时线程（SCHED_FIFO） + 无锁队列** 在 ROS 2 中的性能表现，支持两种
无锁数据结构（`SPSCQueue` 和 `SPSCRingBuffer`），通过文件 IO 模拟硬件读写。

## 架构

```
文件输入 ──→ [read_io_thread] ──→ read_queue ──→ (ROS Topic: read_pub)
                                                       │
                                                       ▼
                                              [LoopbackNode]
                                              (环回转发)
                                                       │
                                                       ▼
文件输出 ←── [write_io_thread] ←── write_queue ←── (ROS Topic: command_data)
```

- **IO 线程**：SCHED_FIFO 实时调度，绑定 CPU 核心 2，独立于 ROS executor
- **环回节点**：订阅后直接转发，验证端到端数据完整性和延迟
- **文件 IO**：以二进制文件模拟硬件，启动时读取、运行中写入、结束时 MD5 校验

## 包结构

| 包名 | 说明 |
|------|------|
| `realtime_common` | 头文件库：`SPSCQueue`、`SPSCRingBuffer`、`rt_utils` |
| `main_interface` | ROS 2 消息定义：`ByteRow`、`ByteRows` |
| `IOqueue` | 基于 SPSC 队列的实时 IO 组件节点 |
| `IOringbuffer` | 基于 SPSC RingBuffer 的实时 IO 组件节点 |
| `test_loopback` | IOqueue 的环回测试节点 |
| `test_ringbuffer_loopback` | IOringbuffer 的环回测试节点 |
| `realtime_launch` | Launch 文件 + 测试数据生成脚本 |

## 依赖

- ROS 2 Humble
- `rclcpp` / `rclcpp_components`
- `cgroup-tools`（CPU 隔离，可选）

```bash
sudo apt install ros-humble-rclcpp ros-humble-rclcpp-components cgroup-tools
```

## 构建

```bash
cd ~/ros2_ws/ros_test
colcon build --packages-select \
    realtime_common main_interface IOqueue IOringbuffer \
    test_loopback test_ringbuffer_loopback realtime_launch
source install/setup.bash
```

## 权限配置

实时线程（SCHED_FIFO）需要 `CAP_SYS_NICE` 能力：

```bash
sudo setcap 'cap_sys_nice,cap_ipc_lock=+ep' /opt/ros/humble/lib/rclcpp_components/component_container
```

或运行提供的脚本：

```bash
bash ld_library_path_to_ldconfig.sh
```

## 运行

### IOqueue 测试

```bash
# 生成测试数据
python3 src/realtime_launch/scripts/generate_test_data.py 10 /tmp/loopback_input.bin

# 运行
ros2 launch realtime_launch realtime.launch.py
```

### IOringbuffer 测试

```bash
# 生成测试数据
python3 src/realtime_launch/scripts/generate_test_data.py 10 /tmp/ringbuffer_input.bin

# 运行
ros2 launch realtime_launch realtime_ringbuffer.launch.py
```

### 数据校验

节点退出后自动 MD5 校验输入输出文件，也可手动验证：

```bash
md5sum /tmp/loopback_input.bin /tmp/loopback_output.bin
md5sum /tmp/ringbuffer_input.bin /tmp/ringbuffer_output.bin
```

## CPU 隔离（可选）

通过 cgroup cpuset 将核心 2 保留给实时线程：

```bash
sudo bash src/realtime_launch/scripts/setup_cpu_isolation.sh
```

或在内核参数中设置 `isolcpus=2`（物理机推荐）。

## 关键参数

| 参数 | 默认值 | 说明 |
|------|--------|------|
| `read_io_priority` | 90 | 读 IO 线程实时优先级 (1-99) |
| `write_io_priority` | 90 | 写 IO 线程实时优先级 (1-99) |
| `read_cpu_core` | 2 | 读 IO 线程绑定的 CPU 核心 |
| `write_cpu_core` | 2 | 写 IO 线程绑定的 CPU 核心 |
| `io_period_ms_read` | 0.02 | 读 IO 循环周期 (ms) |
| `io_period_ms_write` | 0.02 | 写 IO 循环周期 (ms) |
| `read_file` | — | 模拟硬件读取的输入文件路径 |
| `write_file` | — | 模拟硬件写入的输出文件路径 |

## 两种实现对比

| | SPSCQueue | SPSCRingBuffer |
|---|---|---|
| 索引方式 | read/write 原子指针 | per-slot sequence 序列号 |
| 容量要求 | 2 的幂 | 2 的幂 |
| 批量操作 | `pop_bulk` | `pop_batch` |
| 超时等待 | 支持（HYBRID/SPIN/SLEEP） | 不支持 |
| False sharing | 读写指针分置不同 cache line | 每 slot 独立 cache line 对齐 |
| 适用场景 | 吞吐优先、需超时等待 | 延迟敏感、高频读写 |

## License

MIT
