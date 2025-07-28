#pragma once

#include "Core/Logger.hpp"

#include <format>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

namespace AE
{
    template <typename... Args>
    void Logger::Log(LogLevel level, std::string_view format, Args&&... args)
    {
        std::string formattedMsg = std::vformat(format, std::make_format_args(args...));
    
        std::string currContext = _contextStack.empty() ? "" : _contextStack.top();
        
        std::string levelStr, levelColor;
        switch (level)
        {
        case LogLevel::Debug:
            levelStr = "DEBUG";
            levelColor = "\033[36m";
            break;
        case LogLevel::Info:
            levelStr = "INFO";
            levelColor = "\033[32m";
            break;
        case LogLevel::Warning:
            levelStr = "WARN";
            levelColor = "\033[33m";
            break;
        case LogLevel::Error:
            levelStr = "ERROR";
            levelColor = "\033[31m";
            break;
        default:
            levelStr = "UNKNOWN";
            levelColor = "\033[0m";
            break;
        }
    
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm = *std::localtime(&time);
        
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        std::string timeStamp = oss.str();
    
        std::printf("%s%s | [%s] [%s] > %s\033[0m\n", levelColor.c_str(), timeStamp.c_str(), levelStr.c_str(), currContext.c_str(), formattedMsg.c_str());
    }
    
    template <typename... Args>
    void Logger::Debug(std::string_view format, Args&&... args)
    {
        Log(LogLevel::Debug, format, std::forward<Args>(args)...);
    }
    
    template <typename... Args>
    void Logger::Info(std::string_view format, Args&&... args)
    {
        Log(LogLevel::Info, format, std::forward<Args>(args)...);
    }
    
    template <typename... Args>
    void Logger::Warning(std::string_view format, Args&&... args)
    {
        Log(LogLevel::Warning, format, std::forward<Args>(args)...);
    }
    
    template <typename... Args>
    void Logger::Error(std::string_view format, Args&&... args)
    {
        Log(LogLevel::Error, format, std::forward<Args>(args)...);
    }
}