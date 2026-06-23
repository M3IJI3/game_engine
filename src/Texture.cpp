#include "Texture.h"
#include "Logger.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::~Texture() {
    if(m_textureID != 0){
        glDeleteTextures(1, &m_textureID);
    }
}

bool Texture::Load(const std::string& filePath){
    // 1. 用 stb_image 加载图片
    stbi_set_flip_vertically_on_load(true); // OpenGL 坐标系 Y 轴朝上

    unsigned char* data = stbi_load(filePath.c_str(), &m_width, &m_height, &m_channels, 0);

    if(!data){
        Logger::Error("Failed to load textures: " + filePath);
        return false;
    }

    Logger::Info("Loaded texture: " + filePath + " (" + std::to_string(m_width) + "x" + std::to_string(m_height) + ")");

    // 2. 生成OpenGL纹理
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // 3. 设置纹理参数 (缩放和环绕样式)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 4. 根据通道数决定纹理样子
    GLenum format = GL_RGB;
    if(m_channels == 1) format = GL_RED;
    else if(m_channels == 3) format = GL_RGB;
    else if(m_channels == 4) format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

    // 5. 生成多级渐远纹理 (提升渲染性能)
    glGenerateMipmap(GL_TEXTURE_2D);

    // 6. 释放 CPU 上的图片数据
    stbi_image_free(data);

    return true;
}

void Texture::Bind(unsigned int unit) const {
    // 激活指定纹理单元, 绑定纹理
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}