#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D uTexture;
uniform vec3 uLightPos;
uniform float uAmbientIntensity;
uniform float uSpecularIntensity;
uniform vec3 uColor;      // 新增：用于控制物体颜色

void main() {
    vec3 baseColor = uColor;

    // 如果有纹理，用纹理颜色
    vec4 texColor = texture(uTexture, TexCoord);
    if (texColor.a > 0.0) {
        baseColor = texColor.rgb;
    }

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diffuse = max(dot(normal, lightDir), 0.0);

    float ambient = uAmbientIntensity;

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    float specular = spec * uSpecularIntensity;

    float brightness = ambient + diffuse * (1.0 - ambient) + specular;

    FragColor = vec4(baseColor * brightness, 1.0);
}