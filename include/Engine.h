#pragma once

#include "Window.h"
#include "Shader.h"
#include "Scene.h"
#include "Texture.h"
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
    Scene m_scene;
    Texture m_texture;

    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;

    bool m_isRunning = false;
};