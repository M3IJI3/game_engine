#include "Engine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
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
    auto startTime = std::chrono::high_resolution_clock::now();

    while (m_isRunning && !m_window.ShouldClose()) {
        // 计算从启动到现在经过了多少步
        auto currentTime = std::chrono::high_resolution_clock::now();
        float elapsed = std::chrono::duration<float>(currentTime - startTime).count();
        
        m_window.PollEvents();   // 处理键盘/鼠标事件

        // ===== 键盘控制 =====
        float speed = 0.02f;
        GLFWwindow* handle = m_window.GetHandle(); // 先获取窗口句柄
        if(glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS){
            m_position.y += speed;
        }
        if(glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS){
            m_position.y -= speed;
        }
        if(glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS){
            m_position.x -= speed;
        }
        if(glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS){
            m_position.x += speed;
        }

        // ========================

        // ===== 敌人追踪玩家 =====
        // 计算玩家到敌人的方向向量
        glm::vec2 direction = m_position - m_enemyPos;
        float length = glm::length(direction); // 计算距离

        // 如果敌人和玩家的距离大于 0.01 (防止抖动), 就向玩家靠近
        if(length > 0.01f){
            glm::vec2 normalized = direction / length; // 归一化, 得到单位方向
            float enemySpeed = 0.015f;
            m_enemyPos += normalized * enemySpeed;
        }

        // 边界限制 (玩家)
        if(m_position.x >  0.9f) m_position.x =  0.9f;
        if(m_position.x < -0.9f) m_position.x = -0.9f;
        if(m_position.y >  0.9f) m_position.y =  0.9f;
        if(m_position.y < -0.9f) m_position.y = -0.9f;

        // 敌人边界限制
        if(m_enemyPos.x >  0.9f) m_position.x =  0.9f;
        if(m_enemyPos.x < -0.9f) m_position.x = -0.9f;
        if(m_enemyPos.y >  0.9f) m_position.y =  0.9f;
        if(m_enemyPos.y < -0.9f) m_position.y = -0.9f;
        // ======================

        m_window.Clear();        // 清屏为蓝色

        // 创建一个旋转矩阵: 绕着z轴旋转, 角度随时间增加
        // glm::rotate(单位矩阵, 弧度, 旋转轴)
        // glm::mat4 transform = glm::rotate(glm::mat4(1.0f), elapsed * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
        
        // ===== 画玩家（橙色三角形） =====
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 0.0f));
        transform = glm::rotate(transform, elapsed * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
        m_shader.Use();                                // 激活着色器

        glBindVertexArray(m_VAO);                      // 绑定 VAO        
        m_shader.SetVec3("uColor", glm::vec3(1.0f, 0.5f, 0.2f));
        m_shader.SetMat4("uTransform", transform);     // 把矩阵传给着色器里的 uTransform
        glDrawArrays(GL_TRIANGLES, 0, 3);              // 画 3 个顶点 = 1 个三角形

        // ===== 画敌人（蓝色三角形） =====
        glm::mat4 enemyTransform = glm::translate(glm::mat4(1.0f), glm::vec3(m_enemyPos.x, m_enemyPos.y, 0.0f));
        enemyTransform = glm::rotate(enemyTransform, -elapsed * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
        m_shader.SetVec3("uColor", glm::vec3(0.2f, 0.3f, 1.0f));
        m_shader.SetMat4("uTransform", enemyTransform);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // =============================

        m_window.SwapBuffers();  // 把画好的画面显示到屏幕上
    }
}

void Engine::Shutdown() {
    m_isRunning = false;  // 停止主循环
}