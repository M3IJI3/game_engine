#include "Window.h"
#include "Shader.h"
#include <string.h>
#include <iostream>
#include <GLFW/glfw3.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const std::string WINDOW_TITLE = "My Engine";

int main(){
    // 创建窗口对象
    Window window;

    // 创建窗口（宽度800，高度600，标题"My Engine"）
    if(!window.Create(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)){
        return -1;  // 创建失败退出
    }

    // 创建着色器对象并加载
    Shader shader;
    if(!shader.Load("../shaders/vertex.glsl", "../shaders/fragment.glsl")){
        std::cerr << "Shader loading failed..." << std::endl;   
        return -1;
    }

    // 主循环: 只要窗口没关闭, 就一直运行
    while(!window.ShouldClose()){
        window.PollEvents(); // 处理键盘/鼠标事件
        // 将来这里会放 Update() 更新逻辑
        // 将来这里会放 Render() 渲染逻辑
        window.Clear();
        window.SwapBuffers(); // 交换前后缓冲
    }

    return 0;
};