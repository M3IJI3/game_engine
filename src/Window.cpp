#include "Window.h"
#include <GLFW/glfw3.h>
#include <iostream>                     // 用于打印错误信息

// 析构函数: 窗口销毁时自动清理
Window::~Window(){
    std::cout << "Window destructor called" << std::endl;  // 加这行
    // 如果窗口指针不是空的, 销毁窗口并终止GLFW
    if(m_window){
        glfwDestroyWindow(m_window);    // 销毁窗口
        glfwTerminate();                // 终止GLFW
    }
};

// 创建窗口函数
bool Window::Create(int width, int height, const std::string& title){
    // 1. 初始化 GLFW 库
    if(!glfwInit()){
        std::cerr << "GLFW init failed!" << std::endl;
        return false;
    }

    // 2. 配置 OpenGL 版本 (使用3.3核心模式)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // 主版本号 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // 次版本号 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 核心模式

    // 3. 创建窗口
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!m_window){
        std::cerr << "Window creation failed!" << std::endl;
        glfwTerminate();  // 失败要清理 GLFW
        return false;
    }

    // 4. 将窗口设置为当前 OpenGL 上下文
    glfwMakeContextCurrent(m_window);
    
    // 5. 启用垂直同步（限制帧率到显示器刷新率，防止画面撕裂）
    glfwSwapInterval(1);
    
    return true;
}

// 检查用户是否点了窗口的关闭按钮
bool Window::ShouldClose() const { return glfwWindowShouldClose(m_window); }

// 处理所有待处理的时间 (键盘, 鼠标, 窗口大小变化等)
void Window::PollEvents() { glfwPollEvents(); }

// 交换前后缓冲, 把画好的画面显示到屏幕上
void Window::SwapBuffers() { glfwSwapBuffers(m_window); }

// 实现清屏
void Window::Clear(){
    if(!m_window) return;
    // ===== 清屏为蓝色 =====
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
