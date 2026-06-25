cmake_minimum_required(VERSION 3.16)
project(MyEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DGL_SILENCE_DEPRECATION)

include_directories(include)
include_directories(/opt/homebrew/opt/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(third_party/imgui)
include_directories(third_party/imgui/backends)

set(SOURCES
        src/main.cpp
        src/Window.cpp
        src/Shader.cpp
        src/Engine.cpp
        src/Logger.cpp
        src/Scene.cpp
        src/Input.cpp
        src/Texture.cpp
        src/glad.c
        src/imgui.cpp
        src/imgui_draw.cpp
        src/imgui_widgets.cpp
        src/imgui_tables.cpp
        src/imgui_impl_glfw.cpp
        src/imgui_impl_opengl3.cpp
        include/ModelLoader.h
        src/ModelLoader.cpp
        include/Grid.h
        src/Grid.cpp
)

add_executable(MyEngine ${SOURCES})

target_link_libraries(MyEngine
        /opt/homebrew/opt/glfw/lib/libglfw.3.dylib
)

target_link_options(MyEngine PRIVATE -framework OpenGL)