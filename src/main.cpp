#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Window.h"
#include "Shader.h"
#include "Engine.h"
#include <string.h>
#include <iostream>

int main(){
    Engine engine;

    if(!engine.Initialize(800, 600, "My Engine")){
        return -1;
    }
    
    engine.Run();
    engine.Shutdown();

    return 0;
};