#!/bin/bash
# setup_cpu_isolation.sh
# 使用 cgroup v1 cpuset 隔离 CPU 核 2，确保非实时任务不会调度到核 2
# 同时将 IRQ 中断从核 2 移开，提高实时性能

set -e

ISOLATED_CORE=2
CPUSET_BASE="/sys/fs/cgroup/cpuset"
TOTAL_CPUS=$(nproc)

# 构建 system cpuset 的 CPU 列表（排除核 2）
SYSTEM_CPUS=""
for ((i=0; i<TOTAL_CPUS; i++)); do
    if [ "$i" -ne "$ISOLATED_CORE" ]; then
        if [ -z "$SYSTEM_CPUS" ]; then
            SYSTEM_CPUS="$i"
        else
            SYSTEM_CPUS="$SYSTEM_CPUS,$i"
        fi
    fi
done

echo "========================================"
echo "  CPU Isolation: Core $ISOLATED_CORE"
echo "  System CPUs : $SYSTEM_CPUS"
echo "  RT CPUs     : $ISOLATED_CORE,$((ISOLATED_CORE+1))"
echo "========================================"

# 1. 确保 cpuset cgroup 已挂载
if ! mountpoint -q "$CPUSET_BASE" 2>/dev/null; then
    echo "[1/4] Mounting cpuset cgroup..."
    if ! mount -t cgroup -o cpuset cpuset "$CPUSET_BASE" 2>/dev/null; then
        echo "WARN: Cannot mount cpuset (maybe cgroup v2?), trying cgroup v2..."
        CPUSET_BASE="/sys/fs/cgroup"
        # cgroup v2: 检查 unified hierarchy
        if [ -f "$CPUSET_BASE/cgroup.controllers" ]; then
            echo "      Detected cgroup v2 unified hierarchy"
            # 启用 cpuset controller
            echo "+cpuset" > "$CPUSET_BASE/cgroup.subtree_control" 2>/dev/null || true
        else
            echo "ERROR: Cannot setup cpuset isolation"
            exit 1
        fi
    fi
    echo "      OK"
fi

# 2. 创建 system cpuset（所有核除了核 2）
echo "[2/4] Creating system cpuset (all cores except $ISOLATED_CORE)..."
if [ "$CPUSET_BASE" = "/sys/fs/cgroup" ]; then
    # cgroup v2
    if [ ! -d "$CPUSET_BASE/system" ]; then
        mkdir -p "$CPUSET_BASE/system"
    fi
    echo "$SYSTEM_CPUS" > "$CPUSET_BASE/system/cpuset.cpus" 2>/dev/null || \
    echo "$SYSTEM_CPUS" > "$CPUSET_BASE/system/cpuset.cpus.effective" 2>/dev/null || true
    echo 0 > "$CPUSET_BASE/system/cpuset.mems" 2>/dev/null || true
else
    # cgroup v1
    if [ ! -d "$CPUSET_BASE/system" ]; then
        mkdir -p "$CPUSET_BASE/system"
    fi
    echo "$SYSTEM_CPUS" > "$CPUSET_BASE/system/cpuset.cpus"
    echo 0 > "$CPUSET_BASE/system/cpuset.mems"
fi
echo "      Created with CPUs: $SYSTEM_CPUS"

# 3. 将所有当前任务移入 system cpuset（排除核 2）
echo "[3/4] Moving all tasks to system cpuset..."
MOVED=0
FAILED=0
ROOT_PROCS_FILE=""
if [ "$CPUSET_BASE" = "/sys/fs/cgroup" ]; then
    ROOT_PROCS_FILE="$CPUSET_BASE/cgroup.procs"
    DEST_PROCS_FILE="$CPUSET_BASE/system/cgroup.procs"
else
    ROOT_PROCS_FILE="$CPUSET_BASE/cpuset.procs"
    DEST_PROCS_FILE="$CPUSET_BASE/system/cpuset.procs"
fi

for pid in $(cat "$ROOT_PROCS_FILE" 2>/dev/null); do
    if echo "$pid" > "$DEST_PROCS_FILE" 2>/dev/null; then
        ((MOVED++))
    else
        ((FAILED++))
    fi
done
echo "      Moved $MOVED tasks, $FAILED kernel threads skipped"

# 4. 创建 rt cpuset（核 2,3 供实时容器使用）
echo "[4/4] Creating rt cpuset (cores $ISOLATED_CORE,$((ISOLATED_CORE+1)))..."
if [ ! -d "$CPUSET_BASE/rt" ]; then
    mkdir -p "$CPUSET_BASE/rt"
fi
if [ "$CPUSET_BASE" = "/sys/fs/cgroup" ]; then
    # cgroup v2
    echo "$ISOLATED_CORE,$((ISOLATED_CORE+1))" > "$CPUSET_BASE/rt/cpuset.cpus" 2>/dev/null || \
    echo "$ISOLATED_CORE,$((ISOLATED_CORE+1))" > "$CPUSET_BASE/rt/cpuset.cpus.effective" 2>/dev/null || true
    echo 0 > "$CPUSET_BASE/rt/cpuset.mems" 2>/dev/null || true
else
    # cgroup v1
    echo "$ISOLATED_CORE,$((ISOLATED_CORE+1))" > "$CPUSET_BASE/rt/cpuset.cpus"
    echo 0 > "$CPUSET_BASE/rt/cpuset.mems"
fi
echo "      Created with CPUs: $ISOLATED_CORE,$((ISOLATED_CORE+1))"

# 5. 设置 IRQ 亲和性，将中断从核 2 移开
echo ""
echo "[IRQ] Moving IRQ affinity away from core $ISOLATED_CORE..."
IRQ_MASK=0
for ((i=0; i<TOTAL_CPUS; i++)); do
    if [ "$i" -ne "$ISOLATED_CORE" ]; then
        IRQ_MASK=$((IRQ_MASK | (1 << i)))
    fi
done
IRQ_MASK_HEX=$(printf "%x" "$IRQ_MASK")
IRQ_COUNT=0
for irq_dir in /proc/irq/*/; do
    if [ -f "${irq_dir}smp_affinity" ]; then
        echo "$IRQ_MASK_HEX" > "${irq_dir}smp_affinity" 2>/dev/null && ((IRQ_COUNT++)) || true
    fi
done
echo "      Updated $IRQ_COUNT IRQ affinities"

echo ""
echo "========================================"
echo "  CPU Isolation: DONE"
echo "  Core $ISOLATED_CORE is now reserved"
echo "  for real-time tasks only."
echo "========================================"
