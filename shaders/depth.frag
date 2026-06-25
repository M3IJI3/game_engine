//#version 330 core
//
//void main() {
//    // 深度值自动写入，不需要任何输出
//}
#version 330 core

out vec4 FragColor;

void main() {
    float depth = gl_FragCoord.z;
    FragColor = vec4(vec3(depth), 1.0);
}