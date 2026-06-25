//#version 330 core
//
//layout (location = 0) in vec3 aPos;
//
//uniform mat4 uLightSpaceMatrix;
//uniform mat4 uModel;
//
//void main() {
//    gl_Position = uLightSpaceMatrix * uModel * vec4(aPos, 1.0);
//}

#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 uLightSpaceMatrix;
uniform mat4 uModel;

void main() {
    vec4 worldPos = uModel * vec4(aPos, 1.0);
    gl_Position = uLightSpaceMatrix * worldPos;
    // 强制输出，确保顶点数据被处理
}