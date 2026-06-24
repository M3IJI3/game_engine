#define GLFW_INCLUDE_NONE

#include "Engine.h"

int main(){
    Engine engine;

    if(!engine.Initialize(800, 600, "My Engine")){
        return -1;
    }
    
    engine.Run();
    engine.Shutdown();

    return 0;
};