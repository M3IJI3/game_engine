#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Engine.h"
#include "Logger.h"
#include "Scene.h"
#include "Input.h"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <iostream>

bool Engine::Initialize(int width, int height, const std::string& title) {
    // 创建窗口
    if (!m_window.Create(width, height, title)) {
        return false;
    }

    // 初始化 GLAD：加载所有 OpenGL 函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::Error("Failed to initialize GLAD");
        return false;
    }
    Logger::Info("GLAD initialized");

    // 加载着色器（顶点着色器 + 片元着色器）
    if (!m_shader.Load("../shaders/vertex.glsl", "../shaders/fragment.glsl")) {
        Logger::Error("Shader loading failed.");
        return false;
    }

    // 加载纹理图片
    if(!m_texture.Load("../textures/test.jpg")){
        Logger::Error("Failed to load texture");
        return false;
    }

    // 创建三角形顶点数据
    float vertices[] = {
        // 位置               // 纹理坐标
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,  0.5f, 1.0f
    };

    // 创建 VAO（顶点数组对象）：记录数据格式
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // 创建 VBO（顶点缓冲对象）：把数据上传到 GPU
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 告诉 GPU 如何解读数据：位置0，3个float，不标准化，步长12字节，偏移0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 初始化场景
    // m_scene.AddPlayer(glm::vec2(0.0f, 0.0f));
    // m_scene.AddEnemy(glm::vec2(0.5f, 0.5f));

    m_isRunning = true;
    return true;
}

void Engine::Run() {
    auto startTime = std::chrono::high_resolution_clock::now();

    while (m_isRunning && !m_window.ShouldClose()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float elapsed = std::chrono::duration<float>(currentTime - startTime).count();

        m_window.PollEvents();
        Input::GetInstance().Update(m_window.GetHandle());

        // ===== 键盘控制 =====
        float speed = 0.02f;
        Input& input = Input::GetInstance();
        // GLFWwindow* handle = m_window.GetHandle(); // 移除旧控制
        glm::vec2 playerPos = m_scene.GetPlayerPosition();

        if (input.IsKeyPressed(GLFW_KEY_W)) {
            playerPos.y += speed;
        }
        if (input.IsKeyPressed(GLFW_KEY_S)) {
            playerPos.y -= speed;
        }
        if (input.IsKeyPressed(GLFW_KEY_A)) {
            playerPos.x -= speed;
        }
        if (input.IsKeyPressed(GLFW_KEY_D)) {
            playerPos.x += speed;
        }

        // 边界限制
        if (playerPos.x > 0.9f) playerPos.x = 0.9f;
        if (playerPos.x < -0.9f) playerPos.x = -0.9f;
        if (playerPos.y > 0.9f) playerPos.y = 0.9f;
        if (playerPos.y < -0.9f) playerPos.y = -0.9f;

        m_scene.SetPlayerPosition(playerPos);

        // ===== 更新场景 =====
        m_scene.Update();

        // ===== 渲染 =====
        m_window.Clear();

        const auto& objects = m_scene.GetObjects();
        // for (const auto& obj : objects) {
        //     glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(obj.position.x, obj.position.y, 0.0f));
        //     transform = glm::rotate(transform, elapsed * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));

        //     m_shader.Use();
        //     m_texture.Bind(0);              // 绑定纹理到纹理单元0
        //     m_shader.SetInt("uTexture", 0); // 告诉着色器用纹理单元0
        //     if (obj.isPlayer) {
        //         m_shader.SetVec3("uColor", glm::vec3(1.0f, 0.5f, 0.2f));  // 橙色
        //     } else {
        //         m_shader.SetVec3("uColor", glm::vec3(0.2f, 0.3f, 1.0f));  // 蓝色
        //     }
        //     m_shader.SetMat4("uTransform", transform);
        //     glBindVertexArray(m_VAO);
        //     glDrawArrays(GL_TRIANGLES, 0, 3);
        // }

        // 纹理测试: 画一个静态的三角形
        m_shader.Use();
        m_texture.Bind(0);
        m_shader.SetInt("uTexture", 0);

        // 单位矩阵：不旋转、不移动
        glm::mat4 transform = glm::mat4(1.0f);
        m_shader.SetMat4("uTransform", transform);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        m_window.SwapBuffers();
    }
}

void Engine::Shutdown() {
    m_isRunning = false;  // 停止主循环
}