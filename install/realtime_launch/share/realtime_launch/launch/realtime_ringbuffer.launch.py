from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode


def generate_launch_description():
    # isolcpus=2 在内核层面已将普通进程排除在核2之外
    # taskset 将容器绑定到核 2,3，实时线程通过代码再精确绑定到核 2
    container = ComposableNodeContainer(
        name="realtime_container",
        namespace="",
        package="rclcpp_components",
        # executable="component_container_mt",  # 多线程 executor
        executable="component_container",
        arguments=["--isolated"],
 #       prefix="taskset -c 2,3",
        composable_node_descriptions=[
            ComposableNode(
                package="test_ringbuffer_loopback",
                plugin="test_ringbuffer_loopback::RingbufferLoopbackNode",
                name="ringbuffer_loopback_node",
                extra_arguments=[{"use_intra_process_comms": True}],
            ),
            ComposableNode(
                package="IOringbuffer",
                plugin="io_ringbuffer::IOringbuffer",
                name="IOringbuffer",
                parameters=[{
                    "read_io_priority": 90,
                    "write_io_priority": 90,
                    "read_cpu_core": 2,
                    "write_cpu_core": 2,
                    "io_period_ms_read": 0.02,
                    "io_period_ms_write": 0.02,
                    "read_file": "/tmp/ringbuffer_input.bin",
                    "write_file": "/tmp/ringbuffer_output.bin",
                }],
                extra_arguments=[{"use_intra_process_comms": True}],
            ),
        ],
        output="screen",
    )

    return LaunchDescription([container])
