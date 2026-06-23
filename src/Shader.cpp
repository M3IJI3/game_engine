#include "Shader.h"
#include <fstream>      // 读文件
#include <sstream>      // 字符串流
#include <iostream>     // 打印
#include <GLFW/glfw3.h>

// TODO: 析构函数
Shader::~Shader(){}

// 加载着色器: 从文件读取源码
bool Shader::Load(const std::string& vertexPath, const std::string& fragmentPath){ 
    // 1. 从文件读取源码
    std::ifstream vFile(vertexPath);
    std::ifstream fFile(fragmentPath);

    if(!vFile.is_open() || !fFile.is_open()){
        std::cerr << "Fail to open shader files!" << std::endl;
        return false;
    }

    std::stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();

    std::string vCode = vStream.str();
    std::string fCode = fStream.str();

    // 暂时只打印源码长度, 确认读取成功
    // std::cout << "Vertex shader size: " << vCode.size() << " bytes" << std::endl;
    // std::cout << "Fragment shader size: " << fCode.size() << " bytes" << std::endl;

    // 2. 创建顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vSrc = vCode.c_str();
    glShaderSource(vertexShader, 1, &vSrc, nullptr);
    glCompileShader(vertexShader);

    // 检查顶点着色器是否编译成功
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
        return false;
    }
    std::cout << "Vertex shader compiled successfully!" << std::endl;

    // 3. 创建片元着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSrc = fCode.c_str();
    glShaderSource(fragmentShader, 1, &fSrc, nullptr);
    glCompileShader(fragmentShader);

    // 检查片元着色器是否编译成功
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
        return false;
    }
    std::cout << "Fragment shader compiled successfully!" << std::endl;

    // 4. 链接两个着色器成一个程序
    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexShader);
    glAttachShader(m_programID, fragmentShader);
    glLinkProgram(m_programID);

    // 检查链接是否成功
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed:\n" << infoLog << std::endl;
        return false;
    }
    std::cout << "Shader program linked successfully!" << std::endl;

    // 5. 着色器已经链接到程序里, 单独的着色器对象可以删除
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

// 使用着色器
void Shader::Use() const{ glUseProgram(m_programID); }

// 把矩阵传到CPU
void Shader::SetMat4(const std::string& name, const glm::mat4& value){
    unsigned int location = glGetUniformLocation(m_programID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value){
    unsigned int location = glGetUniformLocation(m_programID, name.c_str());
    glUniform3fv(location, 1, &value[0]);
}