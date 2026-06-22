#pragma once

#include <string>

// Shader Class: 负责管理 GPU 上的着色器程序
class Shader{
public:
    // 构造器函数: 默认啥也不做, 着色器 ID 初始为 0
    Shader() = default;
    // 析构函数: 释放 GPU 上的着色器资源
    ~Shader();

    // 从文件加载顶点着色器和片元着色器
    //   vertexPath: 顶点着色器文件路径
    // fragmentPath: 片元着色器文件路径
    bool Load(const std::string& vertexPath, const std::string& fragmentPath);

    // 激活这个着色器程序 (告诉 GPU 用这个着色器渲染)
    void Use() const;
private:
    // GPU 上的着色器程序ID, 0 表示无效
    unsigned int m_programID = 0;
};