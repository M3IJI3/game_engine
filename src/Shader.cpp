#include "Shader.h"
#include <fstream>      // 读文件
#include <sstream>      // 字符串流
#include <iostream>     // 打印

// TODO: 析构函数
Shader::~Shader(){}

// 加载着色器: 从文件读取源码
bool Shader::Load(const std::string& vertexPath, const std::string& fragmentPath){ 
    // 1. 从文件读取着色器源码
    std::ifstream vFile(vertexPath);
    std::ifstream fFile(fragmentPath);

    if(!vFile.is_open() || !fFile.is_open()){
        std::cerr << "Fail to open shader files!" << std::endl;
        return false;
    }

    // 2. 把文件内容读成字符串流
    std::stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();

    std::string vCode = vStream.str();
    std::string fCode = fStream.str();

    // 暂时只打印源码长度, 确认读取成功
    std::cout << "Vertex shader size: " << vCode.size() << " bytes" << std::endl;
    std::cout << "Fragment shader size: " << fCode.size() << " bytes" << std::endl;

    return true;
}

// 使用着色器
void Shader::Use() const{}