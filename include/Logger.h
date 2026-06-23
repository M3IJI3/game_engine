#pragma once
 
#include <string>
#include <iostream>

// 日志登记: 数字越小越重要
enum class LogLevel{
    INFO,           // 普通信息
    WARNING,        // 警告
    ERROR           // 错误
};

class Logger{
public:
    static void Log(LogLevel level, const std::string& message);

    // 快捷函数
    static void Info(const std::string& message);
    static void Warning(const std::string& message);
    static void Error(const std::string& message);
};