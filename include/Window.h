// ensure this header file is included only once
#pragma once

#include <string>

struct GLFWwindow;

// Window Class: 负责管理游戏窗口
class Window{
public:
    Window() = default;
    ~Window(); // 析构函数: 窗口关闭时自动清理资源

    // 创建窗口: 传入宽度, 高度, 窗口标题
    // 成功返回 true, 失败返回 false
    bool Create(int width, int height, const std::string& title);

    // 检查用户是否点击了关闭按钮
    // const: 保证这个函数只能读取成员变量, 不能修改
    bool ShouldClose() const;

    // 处理鼠标, 键盘等事件
    void PollEvents();

    // 交换前后缓冲, 把画好的画面显示到屏幕上
    void SwapBuffers();

    // 清屏
    void Clear();

    // 返回GLFW窗口句柄
    GLFWwindow* GetHandle() const;

private:
    // GLFW窗口指针: 指向真正的窗口对象
    GLFWwindow* m_window = nullptr;
};