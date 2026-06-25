#pragma once

#include "ModelLoader.h"
#include "Window.h"
#include "Shader.h"
#include "Scene.h"
#include "Texture.h"
#include "Grid.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>

class Engine{
public:
    Engine() = default;
    ~Engine() = default;

    bool Initialize(int width, int height, const std::string& title);
    void Run();
    void Shutdown();

private:
    Window m_window;
    Shader m_shader;
    // Shader m_gridShader;
    Scene m_scene;
    Texture m_texture;
    Grid m_grid;

    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;

    bool m_isRunning = false;

    // Camera
    // float m_cameraAngleX   = 0.0f;
    // float m_cameraAngleY   = 0.0f;
    glm::quat m_cameraRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    float m_cameraDistance = 3.0f;
    glm::vec3 m_focusPoint = glm::vec3(0.0f, 0.5f, 0.0f);

    // 光照参数
    glm::vec3 m_lightPos = glm::vec3(2.0f, 2.0f, 2.0f);
    float m_ambientIntensity = 0.2f;

    // 镜面强度变量
    float m_specularIntensity = 0.5f;

    MeshData m_meshData;
};