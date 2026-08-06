#!/bin/bash
# ld_library_path_to_ldconfig.sh
# 把当前 LD_LIBRARY_PATH 导入系统级动态链接器信任路径

set -e

source /opt/ros/humble/setup.bash
source ./install/setup.sh

sudo setcap 'cap_sys_nice,cap_ipc_lock=+ep' /opt/ros/humble/lib/rclcpp_components/component_container_isolated 

CONF_FILE="/etc/ld.so.conf.d/ld_library_path_import.conf"

# 1. 检查 LD_LIBRARY_PATH 是否为空
if [ -z "$LD_LIBRARY_PATH" ]; then
    echo "❌ 当前 LD_LIBRARY_PATH 为空，请先 source 你的 ROS 2 工作空间 setup.bash"
    echo "   例如：source /opt/ros/humble/setup.bash"
    echo "         source ~/your_ws/install/setup.bash"
    exit 1
fi

echo "📥 当前 LD_LIBRARY_PATH 内容："
echo "$LD_LIBRARY_PATH" | tr ':' '\n'
echo ""

# 2. 过滤并去重
#    - 去掉空项
#    - 去掉系统默认路径（/lib, /usr/lib, /usr/local/lib 等已由 ld.so.conf 默认包含）
#    - 去掉重复项
KEEP_PATHS=$(echo "$LD_LIBRARY_PATH" | tr ':' '\n' | \
    grep -v '^$' | \
    grep -v '^/lib$' | \
    grep -v '^/usr/lib$' | \
    grep -v '^/usr/local/lib$' | \
    grep -v '^/lib64$' | \
    grep -v '^/usr/lib64$' | \
    sort -u)

if [ -z "$KEEP_PATHS" ]; then
    echo "⚠️ 过滤后没有需要写入的自定义路径（全是系统默认路径）"
    exit 0
fi

# 3. 写入配置文件（需要 sudo）
echo "📝 写入配置文件: $CONF_FILE"
sudo bash -c "cat > $CONF_FILE << EOF
# Auto-imported from LD_LIBRARY_PATH on $(date)
# DO NOT EDIT MANUALLY — regenerate with ld_library_path_to_ldconfig.sh
$KEEP_PATHS
EOF"

echo ""
echo "✅ 已写入以下路径："
echo "$KEEP_PATHS"
echo ""

# 4. 刷新动态链接器缓存
echo "🔄 刷新 ldconfig 缓存..."
sudo ldconfig
echo "🔄 刷新 ldconfig 缓存完成  ！！！"
