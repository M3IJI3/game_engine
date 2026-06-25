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
#include <glm/gtc/quaternion.hpp>
#include <chrono>
#include <iostream>
#include <vector>

bool Engine::Initialize(int width, int height, const std::string& title) {
    if (!m_window.Create(width, height, title)) {
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL((m_window.GetHandle()), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    Logger::Info("ImGui initialized");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::Error("Failed to initialize GLAD");
        return false;
    }
    Logger::Info("GLAD initialized");

    if (!m_shader.Load("../shaders/vertex.glsl", "../shaders/fragment.glsl")) {
        Logger::Error("Shader loading failed.");
        return false;
    }

    if(!m_texture.Load("../textures/test.jpg")){
        Logger::Error("Failed to load texture");
        return false;
    }

    m_grid.Generate(100, 10);

    if (!ModelLoader::LoadOBJ("../models/bunny.obj", m_meshData)) {
        Logger::Error("Failed to load model");
        return false;
    }
    Logger::Info("Loaded model: " + std::to_string(m_meshData.positions.size()) + " vertices");

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

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertexData.size() * sizeof(VertexData),
                 vertexData.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, texcoord));
    glEnableVertexAttribArray(1);
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
        double mouseX, mouseY;
        glfwGetCursorPos(m_window.GetHandle(), &mouseX, &mouseY);

        static double lastX = mouseX;
        static double lastY = mouseY;

        float dx = mouseX - lastX;
        float dy = mouseY - lastY;

        if (glfwGetMouseButton(m_window.GetHandle(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glm::quat rotX = glm::angleAxis(dx * 0.003f, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::quat rotY = glm::angleAxis(dy * 0.003f, glm::vec3(1.0f, 0.0f, 0.0f));
            m_cameraRotation = rotY * m_cameraRotation * rotX;
            m_cameraRotation = glm::normalize(m_cameraRotation);
        }

        float scroll = Input::GetInstance().GetScrollOffset();
        m_cameraDistance -= scroll * 0.1f;
        if (m_cameraDistance < 1.0f) m_cameraDistance = 1.0f;
        if (m_cameraDistance > 10.0f) m_cameraDistance = 10.0f;
        Input::GetInstance().SetScrollOffset(0.0f);

        lastX = mouseX;
        lastY = mouseY;

        // ===== 场景更新 =====
        float speed = 0.02f;
        Input& input = Input::GetInstance();
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

        if (playerPos.x > 0.9f) playerPos.x = 0.9f;
        if (playerPos.x < -0.9f) playerPos.x = -0.9f;
        if (playerPos.y > 0.9f) playerPos.y = 0.9f;
        if (playerPos.y < -0.9f) playerPos.y = -0.9f;

        m_scene.SetPlayerPosition(playerPos);
        m_scene.Update();

        // ===== ImGui =====
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debug Panel");
        ImGui::Text("Light Controls");
        ImGui::SliderFloat3("Light Position", &m_lightPos[0], -3.0f, 3.0f);
        ImGui::SliderFloat("Ambient", &m_ambientIntensity, 0.0f, 1.0f);
        ImGui::SliderFloat("Specular Intensity", &m_specularIntensity, 0.0f, 1.0f);
        ImGui::Text("Light Pos: %.2f, %.2f, %.2f", m_lightPos.x, m_lightPos.y, m_lightPos.z);
        ImGui::Text("Ambient: %.2f", m_ambientIntensity);
        ImGui::End();

        // ===== 渲染 =====
        m_window.Clear();

        m_shader.Use();
        m_texture.Bind(0);
        m_shader.SetInt("uTexture", 0);

        glm::mat4 model = glm::mat4(1.0f);

        glm::vec3 focusPoint = m_focusPoint;

        // 用四元数计算摄像机位置（解决万向锁）
        float radius = m_cameraDistance;
        glm::vec3 cameraOffset = glm::vec3(0.0f, 0.0f, radius);
        cameraOffset = m_cameraRotation * cameraOffset;
        glm::vec3 cameraPos = focusPoint + cameraOffset;

        // ===== 键盘控制摄像机平移（沿摄像机方向） =====
        float moveSpeed = 2.0f * (m_cameraDistance / 3.0f);

        // 获取摄像机的朝向向量
        glm::vec3 forward = glm::normalize(focusPoint - cameraPos);
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

        // 让移动方向保持水平（不上下移动）
        forward.y = 0.0f;
        right.y = 0.0f;
        forward = glm::normalize(forward);
        right = glm::normalize(right);

        float step = moveSpeed * 0.016f;
        if (Input::GetInstance().IsKeyPressed(GLFW_KEY_W)) {
            m_focusPoint += forward * step;
        }
        if (Input::GetInstance().IsKeyPressed(GLFW_KEY_S)) {
            m_focusPoint -= forward * step;
        }
        if (Input::GetInstance().IsKeyPressed(GLFW_KEY_A)) {
            m_focusPoint -= right * step;
        }
        if (Input::GetInstance().IsKeyPressed(GLFW_KEY_D)) {
            m_focusPoint += right * step;
        }
        // ==============================================

        // 重新计算 view 矩阵（使用更新后的 focusPoint）
        focusPoint = m_focusPoint;
        cameraPos = focusPoint + cameraOffset;
        glm::mat4 view = glm::lookAt(cameraPos, focusPoint, glm::vec3(0.0f, 1.0f, 0.0f));

        float aspect = 800.0f / 600.0f;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        m_shader.SetMat4("uModel", model);
        m_shader.SetMat4("uView", view);
        m_shader.SetMat4("uProjection", projection);
        m_shader.SetVec3("uLightPos", m_lightPos);
        m_shader.SetFloat("uAmbientIntensity", m_ambientIntensity);
        m_shader.SetFloat("uSpecularIntensity", m_specularIntensity);

        m_grid.Draw(m_shader.GetProgramID(), view, projection, cameraPos);

        // 设置兔子颜色为白色
        m_shader.SetVec3("uColor", glm::vec3(1.0f, 1.0f, 1.0f));

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_meshData.positions.size());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_window.SwapBuffers();
    }
}

void Engine::Shutdown() {
    m_isRunning = false;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}