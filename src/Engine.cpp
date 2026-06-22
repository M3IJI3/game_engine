#include "Engine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

bool Engine::Initialize(int width, int height, const std::string& title) {
    // 1. 创建窗口
    if (!m_window.Create(width, height, title)) {
        return false;
    }

    // 2. 初始化 GLAD：加载所有 OpenGL 函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    std::cout << "GLAD initialized" << std::endl;

    // 3. 加载着色器（顶点着色器 + 片元着色器）
    if (!m_shader.Load("../shaders/vertex.glsl", "../shaders/fragment.glsl")) {
        std::cerr << "Shader loading failed" << std::endl;
        return false;
    }

    // 4. 创建三角形顶点数据
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  // 左下角
         0.5f, -0.5f, 0.0f,  // 右下角
         0.0f,  0.5f, 0.0f   // 顶部
    };

    // 5. 创建 VAO（顶点数组对象）：记录数据格式
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // 6. 创建 VBO（顶点缓冲对象）：把数据上传到 GPU
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 7. 告诉 GPU 如何解读数据：位置0，3个float，不标准化，步长12字节，偏移0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_isRunning = true;
    return true;
}

void Engine::Run() {
    // 主循环：一直运行直到窗口关闭或引擎停止
    while (m_isRunning && !m_window.ShouldClose()) {
        m_window.PollEvents();   // 处理键盘/鼠标事件
        m_window.Clear();        // 清屏为蓝色

        m_shader.Use();          // 激活着色器
        glBindVertexArray(m_VAO); // 绑定 VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // 画 3 个顶点 = 1 个三角形

        m_window.SwapBuffers();  // 把画好的画面显示到屏幕上
    }
}

void Engine::Shutdown() {
    m_isRunning = false;  // 停止主循环
}