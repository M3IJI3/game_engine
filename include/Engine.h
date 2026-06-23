#pragma once

#include "Window.h"
#include "Shader.h"
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

    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;

    float m_time = 0.0f;  // 记录时间, 用来计算旋转角度

    // 三角形位置 (玩家位置)
    glm::vec2 m_position = glm::vec2(0.0f, 0.0f); // x 和 y 的偏移量

    // 新三角形位置 (敌人位置)
    glm::vec2 m_enemyPos = glm::vec2(0.5f, 0.5f); // 右上角开始


    bool m_isRunning = false;
};