#!/usr/bin/env python3
"""
生成环回测试用的输入文件。

用法（在 colcon build 并 source setup.bash 后）:
  python3 src/realtime_launch/scripts/generate_test_data.py [文件大小MB，默认10]

IOqueue 测试:
  python3 src/realtime_launch/scripts/generate_test_data.py 200 /tmp/loopback_input.bin

IOringbuffer 测试:
  python3 src/realtime_launch/scripts/generate_test_data.py 5 /tmp/ringbuffer_input.bin
"""

import sys
import os

def generate_test_data(filepath: str, size_mb: int):
    size_bytes = size_mb * 1024 * 1024
    chunk_size = 65536  # 64KB chunks for efficient write

    print(f"Generating {size_mb}MB test file: {filepath}")

    with open(filepath, 'wb') as f:
        remaining = size_bytes
        counter = 0
        while remaining > 0:
            n = min(chunk_size, remaining)
            # 使用可重复的模式填充，便于校验
            data = bytes([(counter + i) & 0xFF for i in range(n)])
            f.write(data)
            remaining -= n
            counter += n

            if counter % (10 * 1024 * 1024) == 0:
                progress = (size_bytes - remaining) * 100 // size_bytes
                print(f"  Progress: {progress}%")

    actual_size = os.path.getsize(filepath)
    print(f"Done: {actual_size} bytes ({actual_size / 1024 / 1024:.2f} MB)")


if __name__ == '__main__':
    size_mb = int(sys.argv[1]) if len(sys.argv) > 1 else 10
    filepath = sys.argv[2] if len(sys.argv) > 2 else '/tmp/ringbuffer_input.bin'
    generate_test_data(filepath, size_mb)
