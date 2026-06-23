#version 330 core  // 使用OpenGL 3.3版本的着色器语法

// out 表示这是输出数据，从GPU输出到屏幕
// vec4 表示这是一个四维向量 (R, G, B, A) 分别代表红绿蓝和透明度
// FragColor 是变量名，代表这个像素最终显示的颜色
out vec4 FragColor;

// 从 CPU 传颜色
uniform vec3 uColor;

// main函数：每个像素（碎片）都会执行一次这个函数
void main() {
    // vec4(1.0, 0.5, 0.2, 1.0) 表示：
    // 红色 = 1.0（最大值），绿色 = 0.5，蓝色 = 0.2，透明度 = 1.0（不透明）
    // 这个颜色是橙色
    FragColor = vec4(uColor, 1.0);
}