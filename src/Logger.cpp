# include "Logger.h"

void Logger::Log(LogLevel level, const std::string& message){
    switch(level){
        case LogLevel::INFO:
            std::cout << "[INFO] " << message << std::endl;
            break;
        case LogLevel::WARNING:
            std::cout << "[WARNING] " << message << std::endl;
            break;
        case LogLevel::ERROR:
            std::cout << "[ERROR] " << message << std::endl;
            break; 
    }
}

void Logger::Info(const std::string& message){
    Log(LogLevel::INFO, message);
}

void Logger::Warning(const std::string& message){
    Log(LogLevel::WARNING, message);
}

void Logger::Error(const std::string& message){
    Log(LogLevel::ERROR, message);
}