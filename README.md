# 🚀 MyEngine

**MyEngine** 是一个基于 **C++17** 和 **OpenGL 3.3** 从零开始构建的轻量级 3D 游戏引擎。支持模型加载、实时光照、轨道摄像机控制和 ImGui 调试界面。

> 项目目标：深入理解图形渲染管线与游戏引擎架构设计。

---

## ✨ 特性

| 模块 | 功能 |
|------|------|
| 🖼️ **渲染** | 基于 OpenGL 3.3 的渲染管线（VAO / VBO / Shader） |
| 📦 **模型加载** | 集成 `tinyobjloader`，支持 `.obj` 格式模型 |
| ☀️ **光照** | Blinn-Phong 光照模型（环境光 + 漫反射 + 镜面高光） |
| 🎥 **摄像机** | 轨道摄像机（鼠标拖拽旋转 / 滚轮缩放 / WASD 平移） |
| 🧰 **调试界面** | 集成 Dear ImGui，支持运行时调整光照参数 |
| 🧱 **架构** | 模块化设计：Engine / Window / Shader / Scene / Texture / ModelLoader |
| 📐 **辅助** | 地面网格 + 模型自动适配（缩放/居中） |

---

## 🛠️ 技术栈

| 技术 | 说明 |
|------|------|
| C++17 | 核心语言 |
| OpenGL 3.3 | 图形 API |
| GLFW | 窗口与输入管理 |
| GLAD | OpenGL 函数加载 |
| GLM | 数学库（矩阵/向量运算） |
| stb_image | 纹理加载 |
| tinyobjloader | OBJ 模型解析 |
| Dear ImGui | 调试界面 |

---

## 📁 项目结构

## 📁 项目结构

```text
MyEngine/
├── CMakeLists.txt                # CMake 构建配置
├── README.md                     # 项目说明
│
├── include/                      # 头文件 (.h)
│   ├── Engine.h                  # 引擎核心类
│   ├── Window.h                  # 窗口管理
│   ├── Shader.h                  # 着色器管理
│   ├── Scene.h                   # 场景管理
│   ├── Texture.h                 # 纹理管理
│   ├── ModelLoader.h             # OBJ 模型加载
│   ├── Input.h                   # 输入系统
│   ├── Logger.h                  # 日志系统
│   └── Grid.h                    # 地面网格
│
├── src/                          # 源文件 (.cpp)
│   ├── main.cpp                  # 程序入口
│   ├── Engine.cpp
│   ├── Window.cpp
│   ├── Shader.cpp
│   ├── Scene.cpp
│   ├── Texture.cpp
│   ├── ModelLoader.cpp
│   ├── Input.cpp
│   ├── Logger.cpp
│   └── Grid.cpp
│
├── shaders/                      # GLSL 着色器
│   ├── vertex.glsl               # 顶点着色器
│   └── fragment.glsl             # 片元着色器
│
├── textures/                     # 纹理资源
│   └── test.jpg                  # 测试纹理
│
├── models/                       # 3D 模型 (.obj)
│   ├── bunny.obj                 # 斯坦福兔子
│   ├── dragon.obj                # 斯坦福龙
│   └── cube.obj                  # 测试立方体
│
└── third_party/                  # 第三方库（源码）
    ├── imgui/                    # Dear ImGui
    │   ├── imgui.h
    │   ├── imgui.cpp
    │   ├── backends/
    │   │   ├── imgui_impl_glfw.h
    │   │   ├── imgui_impl_glfw.cpp
    │   │   ├── imgui_impl_opengl3.h
    │   │   └── imgui_impl_opengl3.cpp
    │   └── ...
    └── tinyobjloader/            # tinyobjloader
        └── tiny_obj_loader.h
```

---

## 🧪 演示

| 场景 | 说明 |
|------|------|
| 加载斯坦福龙 | 展示模型加载与光照效果 |
| 实时调参 | ImGui 面板调整光源位置、环境光强度、镜面高光 |
| 轨道摄像机 | 鼠标拖拽旋转 / 滚轮缩放 / WASD 平移 |

standford_dragon
<img width="2880" height="1800" alt="dragon" src="https://github.com/user-attachments/assets/cbda47cf-fb04-4aa4-8fa6-b6ccccc91c4e" />


---

## ⚙️ 构建与运行

### 依赖

- CMake 3.16+
- C++17 编译器
- GLFW（macOS: `brew install glfw`，Linux: `apt install libglfw3-dev`，Windows: vcpkg）

### 构建

```bash
git clone https://github.com/M3IJI3/game_engine
cd MyEngine
mkdir build && cd build
cmake ..
make
./MyEngine


