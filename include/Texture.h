#pragma once

#include <string>

class Texture{
public:
    Texture() = default;
    ~Texture();

    // 从文件加载纹理
    bool Load(const std::string& flipPath);

    // 激活纹理, 绑定到指定的纹理单元
    void Bind(unsigned int unit = 0) const;

    // 获取纹理宽度和高度
    int  GetWidth() const { return m_width; }
    int GetHeight() const {return m_height; }

private:
    unsigned int m_textureID = 0;
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0; // 颜色通道数 (RGB = 3, RGBA = 4)
};