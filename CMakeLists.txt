cmake_minimum_required(VERSION 3.16)
project(MyEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DGL_SILENCE_DEPRECATION)

include_directories(include)
include_directories(/opt/homebrew/opt/glfw/include)

set(SOURCES
    src/main.cpp
    src/Window.cpp
    src/Shader.cpp
)

add_executable(MyEngine ${SOURCES})

target_link_libraries(MyEngine 
    /opt/homebrew/opt/glfw/lib/libglfw.3.dylib
)

# 单独链接OpenGL框架
target_link_options(MyEngine PRIVATE -framework OpenGL)