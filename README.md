# dde-dock-privacy

dde-dock 隐私指示器插件，在摄像头、麦克风或位置服务被使用时显示提示图标，类似于 iOS 的隐私指示器。

## 功能特性

- **摄像头监控**：检测摄像头设备是否被访问
- **麦克风监控**：检测音频采集是否处于活动状态
- **位置监控**：检测位置服务是否在使用中
- **视觉指示器**：设备活动时在任务栏显示彩色图标
- **悬停提示**：显示当前正在使用的设备

## 依赖项

- dde-dock-dev（系统 dde-dock 开发包）
- dtkwidget-dev（DTK 开发包）
- qtbase5-dev（Qt5 基础开发包）
- libqt5svg5-dev（Qt5 SVG 支持）
- cmake
- pkg-config

**注意**：系统 dde-dock 当前使用 Qt5，因此本项目使用 Qt5 编译。

## 编译

```bash
mkdir build && cd build
cmake ..
make -j4
```

编译生成的文件：
- `build/libprivacy-indicator.so` - 插件库文件（约 235KB）

## 安装

### 系统范围安装（需要 root 权限）
```bash
sudo make install
```

### 用户本地安装
```bash
mkdir -p ~/.local/lib/dde-dock/plugins
cp libprivacy-indicator.so ~/.local/lib/dde-dock/plugins/
```

## 使用

安装后，重启 dde-dock：
```bash
pkill dde-dock; dde-dock
```

插件将出现在任务栏中并自动监控设备使用情况。

## 许可证

详见 LICENSE 文件。
