#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D uTexture;
uniform vec3 uLightPos;
uniform float uAmbientIntensity;
uniform float uSpecularIntensity;

void main() {
    // 1. 从纹理采样颜色
    vec2 texCoord = FragPos.xy * 0.5 + 0.5;
    vec4 texColor = texture(uTexture, texCoord);

    // ===== 光照计算 =====
    vec3 normal = normalize(Normal);

    // 漫反射：光源方向 = 光源位置 - 物体位置
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diffuse = max(dot(normal, lightDir), 0.0);

    // 环境光
    float ambient = uAmbientIntensity;

    // ===== 新增：镜面高光 =====
    // 视角方向：摄像机在原点，所以从物体指向摄像机 = -FragPos
    vec3 viewDir = normalize(-FragPos);
    // 反射方向：光线经法线反射后的方向
    vec3 reflectDir = reflect(-lightDir, normal);
    // 计算高光强度：视角方向和反射方向越接近，高光越亮
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // 镜面高光 = 高光强度 × 镜面强度系数
    float specular = spec * uSpecularIntensity;
    // =========================

    // 最终亮度 = 环境光 + 漫反射 + 镜面高光
    float brightness = ambient + diffuse * (1.0 - ambient) + specular;

    FragColor = vec4(texColor.rgb * brightness, 1.0);
}