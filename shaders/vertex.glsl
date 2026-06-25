#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uLightSpaceMatrix;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

void main() {
    vec4 worldPos = uModel * vec4(aPos, 1.0);
    gl_Position = uProjection * uView * worldPos;
    TexCoord = aTexCoord;
    FragPos = worldPos.xyz;
    Normal = mat3(transpose(inverse(uModel))) * aNormal;
    FragPosLightSpace = uLightSpaceMatrix * worldPos;
}