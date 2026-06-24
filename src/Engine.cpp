#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Engine.h"
#include "ModelLoader.h"
#include "Logger.h"
#include "Scene.h"
#include "Input.h"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <iostream>
#include <vector>

bool Engine::Initialize(int width, int height, const std::string& title) {
    // 创建窗口
    if (!m_window.Create(width, height, title)) {
        return false;
    }

    // 初始化 ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL((m_window.GetHandle()), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    Logger::Info("ImGui initialized");

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

        // 加载模型
    if (!ModelLoader::LoadOBJ("../models/cube.obj", m_meshData)) {
        Logger::Error("Failed to load model");
        return false;
    }
    Logger::Info("Loaded model: " + std::to_string(m_meshData.positions.size()) + " vertices");

    // // 创建三角形顶点数据
    // float vertices[] = {
    //     // 位置               // 纹理坐标
    //     -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
    //      0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
    //      0.0f,  0.5f, 0.0f,  0.5f, 1.0f
    // };


    // 正方形
    // float vertices[] = {
    //     // 位置               // 纹理坐标
    //     -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // 左下
    //      0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // 右下
    //      0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // 右上

    //      0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // 右上
    //     -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,   // 左上
    //     -0.5f, -0.5f, 0.0f,  0.0f, 0.0f    // 左下
    // };

    // 正方体数据
        float vertices[] = {
        // 位置         // 纹理             // 法线(控制光线反射)
        // 前面 (Z正方向)
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,

        // 后面 (Z负方向)
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,

        // 右面 (X正方向)
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

        // 左面 (X负方向)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,

        // 顶面 (Y正方向)
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,

        // 底面 (Y负方向)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f
    };

    // 创建 VAO（顶点数组对象）：记录数据格式
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // 创建 VBO（顶点缓冲对象）：把数据上传到 GPU
    // glGenBuffers(1, &m_VBO);
    // glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // glBufferData(GL_ARRAY_BUFFER, 
    //          m_meshData.positions.size() * sizeof(glm::vec3),
    //          m_meshData.positions.data(), 
    //          GL_STATIC_DRAW);

    // // 位置(location = 0): 3个float
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // // 纹理坐标 (location = 1): 2个float
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // // 法线(location = 2): 3个float
    // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    // glEnableVertexAttribArray(2);

        // 创建 VBO：只上传位置数据
    // glGenBuffers(1, &m_VBO);
    // glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // glBufferData(GL_ARRAY_BUFFER, 
    //              m_meshData.positions.size() * sizeof(glm::vec3),
    //              m_meshData.positions.data(), 
    //              GL_STATIC_DRAW);

    // // 只有位置属性 (location = 0)
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    // glEnableVertexAttribArray(0);

        // 创建 VBO：上传所有数据（位置 + 纹理坐标 + 法线）
    struct VertexData {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
    };
    
    std::vector<VertexData> vertexData;
    vertexData.resize(m_meshData.positions.size());
    for (size_t i = 0; i < m_meshData.positions.size(); i++) {
        vertexData[i].position = m_meshData.positions[i];
        vertexData[i].texcoord = (i < m_meshData.texcoords.size()) ? m_meshData.texcoords[i] : glm::vec2(0.0f);
        vertexData[i].normal = (i < m_meshData.normals.size()) ? m_meshData.normals[i] : glm::vec3(0.0f, 0.0f, 1.0f);
    }

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 vertexData.size() * sizeof(VertexData),
                 vertexData.data(), 
                 GL_STATIC_DRAW);

    // 位置 (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, position));
    glEnableVertexAttribArray(0);

    // 纹理坐标 (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, texcoord));
    glEnableVertexAttribArray(1);

    // 法线 (location = 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
    glEnableVertexAttribArray(2);

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

        // ===== 鼠标控制 =====
        // double mouseX, mouseY;
        // glfwGetCursorPos(m_window.GetHandle(), &mouseX, &mouseY);
        //
        // static double lastX = mouseX;
        // static double lastY = mouseY;
        //
        // float dx = mouseX - lastX;
        // float dy = mouseY - lastY;
        //
        // // 鼠标左键按住时拖动
        // if (glfwGetMouseButton(m_window.GetHandle(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        //     m_cameraAngleX += dx * 0.005f;
        //     m_cameraAngleY += dy * 0.005f;
        // }
        //
        // // TODO: 鼠标滚轮缩放
        //
        // lastX = mouseX;
        // lastY = mouseY;
        // ===================

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
        if (playerPos.x >  0.9f) playerPos.x =  0.9f;
        if (playerPos.x < -0.9f) playerPos.x = -0.9f;
        if (playerPos.y >  0.9f) playerPos.y =  0.9f;
        if (playerPos.y < -0.9f) playerPos.y = -0.9f;

        m_scene.SetPlayerPosition(playerPos);

        // ===== 更新场景 =====
        m_scene.Update();

        // ===== 开始 ImGui 帧 =====
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 绘制一个简单的调试窗口
        ImGui::Begin("Debug Panel");
        ImGui::Text("Light Controls");

        // 光源位置滑块 (范围 -3 到 3)
        ImGui::SliderFloat3("Light Position", &m_lightPos[0], -3.0f, 3.0f);

        // 环境光强度滑块 (范围 0 到 1)
        ImGui::SliderFloat("Ambient", &m_ambientIntensity, 0.0f, 1.0f);

        ImGui::SliderFloat("Specular Intensity", &m_specularIntensity, 0.0f, 1.0f);

        // 显示当前数值
        ImGui::Text("Light Pos: %.2f, %.2f, %.2f", m_lightPos.x, m_lightPos.y, m_lightPos.z);
        ImGui::Text("Ambient: %.2f", m_ambientIntensity);

        ImGui::End();
        // ========================

        // ===== 渲染 =====
        m_window.Clear();
        
        const auto & objects = m_scene.GetObjects();
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
        // glm::mat4 transform = glm::mat4(1.0f);
        // m_shader.SetMat4("uTransform", transform);

        // 让正方体绕着 Y 轴慢慢旋转
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), elapsed * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

        // 根据鼠标位置控制视角
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::rotate(view, m_cameraAngleX, glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, m_cameraAngleY, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -m_cameraDistance));

        // 投影矩阵: 透视
        float aspect = 800.0f / 600.0f;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        // 上传到着色器
        m_shader.SetMat4("uModel", model);
        m_shader.SetMat4("uView", view);
        m_shader.SetMat4("uProjection", projection);
        m_shader.SetVec3("uLightPos", m_lightPos);
        m_shader.SetFloat("uAmbientIntensity", m_ambientIntensity);
        m_shader.SetFloat("uSpecularIntensity", m_specularIntensity);

        // 光源位置(在物体右上方)
        glm::vec3 lightPos = glm::vec3(2.0f, 2.0f, 2.0f);
        m_shader.SetVec3("uLightPos", lightPos);

        glBindVertexArray(m_VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36); // 正方体
        glDrawArrays(GL_TRIANGLES, 0, m_meshData.positions.size());

        // ===== 渲染 ImGui =====
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // =====================

        m_window.SwapBuffers();
    }
}

void Engine::Shutdown() {
    m_isRunning = false;  // 停止主循环

    // 清理ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}