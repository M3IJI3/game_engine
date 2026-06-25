#include "Input.h"

// 获取全局唯一的 Input 实例
Input& Input::GetInstance(){
    static Input instance;     // 静态局部变量, 程序启动时创建一次
    return instance;
}

static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Input::GetInstance().SetScrollOffset((float)yoffset);
}

// 每帧更新按键状态
void Input::Update(GLFWwindow* window){
    static bool initialized = false;
    if (!initialized) {
        glfwSetScrollCallback(window, ScrollCallback);
        initialized = true;
    }

    // 先把上一帧的 刚按下 状态清掉
    for(int i = 0 ; i < 1024 ; i++){
        m_keyJustPressed[i] = false;
    }

    // 遍历所有键, 更新当前状态
    for(int i = 0 ; i < 1024 ; i++){
        bool isDown = glfwGetKey(window, i) == GLFW_PRESS;

        // 如果这个键上一帧没按下, 这一帧按下了 -> 刚按下
        if(!m_keys[i] && isDown){
            m_keyJustPressed[i] = true;
        }

        m_keys[i] = isDown;
    }
}

// 检查这个键是否持续按住
bool Input::IsKeyPressed(int key) const {
    return m_keys[key];
}

// 检查某个键是否刚按下(仅那帧为 true)
bool Input::IsKeyJustPressed(int key) const {
    return m_keyJustPressed[key];
}

