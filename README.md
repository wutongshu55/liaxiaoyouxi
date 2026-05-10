# 🎮 Windows 控制台小游戏合集

一个基于 Windows 控制台 API 开发的经典小游戏合集，包含贪吃蛇和太空射击两款游戏，并提供图形化启动器。

## 游戏列表

### 1. 贪吃蛇 (Snake Game) — `dyg.exe`
- 经典贪吃蛇玩法，使用 WASD 控制方向
- 随机生成食物，每吃一个加 10 分
- 撞墙或撞自身游戏结束
- 支持游戏中途重新开始 (R) 和退出 (Q)

### 2. 太空射击 (Shooter Game) — `shooter.exe`
- 纵向卷轴射击游戏，玩家控制飞船抵御敌人
- A/D 键左右移动，空格键射击
- 两种敌人：普通敌人 (M) 和精英敌人 (MM)
- 连击系统：连续击杀可获得分数加成
- 动态难度：分数越高，等级提升，敌人刷新更快
- 闪烁星空背景，增强沉浸感
- 游戏结束后根据分数给出评价

## 启动器

提供两种启动器，功能相同：

- **C++ 版** (`Launcher.exe`) — Win32 GUI 窗口
- **Python 版** (`launcher.py`) — Tkinter 图形界面（需要 Python 环境）

## 快速开始

### 直接运行
```bash
# 直接运行游戏
bin/dyg.exe          # 贪吃蛇
bin/shooter.exe      # 太空射击
bin/Launcher.exe     # 启动器

# 或使用 Python 版启动器
python launcher.py
```

### 编译（Visual Studio）
项目使用 Visual Studio 解决方案 (`Project1.sln`)，在 Visual Studio 中直接打开编译即可。

- 贪吃蛇入口: `src/dyg.cpp`
- 太空射击入口: `src/shooter.cpp`
- 启动器入口: `src/launcher.cpp`

## 系统要求
- 操作系统：Windows（依赖 Win32 API）
- 编译器：MSVC（Visual Studio）
- Python 版启动器需要 Python 3 + Tkinter

## 开发说明
- 使用 Windows Console API (`<windows.h>`) 实现控制台绘图
- 所有游戏均为单线程，通过 `Sleep()` 控制帧率
- 项目结构简单，适合 C++ 初学者学习参考
