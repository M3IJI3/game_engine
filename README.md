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
