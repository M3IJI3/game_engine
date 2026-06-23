#pragma once

#include <GLFW/glfw3.h>

class Input{
public:
    // 获取单例实例 (全局只有一个Input)
    static Input& GetInstance();

    // 每帧调用, 更新所有按键的状态
    void Update(GLFWwindow* window);

    // 检查某个键是否被按住(持续按下)
    bool IsKeyPressed(int key) const;

    // 检查某个键是否刚被按下(仅那一帧为 true)
    bool IsKeyJustPressed(int key) const;

private:
    Input() = default; // 私有构造函数, 防止外部创建多个实例

    // 按键状态数组: 1024个键, GLFW支持的最大键数
    bool m_keys[1024] = { false };           // 当前帧是否按住
    bool m_keyJustPressed[1024] = { false }; // 这一帧是否刚按下
};