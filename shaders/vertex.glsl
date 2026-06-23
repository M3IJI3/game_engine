#version 330 core  // 使用OpenGL 3.3版本的着色器语法

// layout (location = 0) 表示这个数据来自第0号缓冲位置
// in 表示这是输入数据，来自CPU传给GPU的顶点数据
// vec3 表示这是一个三维向量 (x, y, z)
// aPos 是变量名，代表顶点的位置坐标
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord; // 纹理坐标

uniform mat4 uTransform;

out vec2 TexCoord; // 传给片元着色器

// main函数：每个顶点都会执行一次这个函数
void main() {
    // gl_Position 是GLSL内置变量，表示顶点在屏幕上的最终位置
    // vec4(aPos.x, aPos.y, aPos.z, 1.0) 把三维坐标转成四维坐标
    // 第四个分量1.0代表这是一个"位置"点，而不是"方向"向量
    // 新增: 用矩阵 * 顶点坐标, 得到变换后的位置
    gl_Position = uTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aTexCoord;
}