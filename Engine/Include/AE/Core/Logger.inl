#pragma once

#include "Core/Logger.hpp"

#include <sstream>
#include <format>

namespace AE
{
    template <typename... Args>
    void Logger::Log(LogLevel level, std::string_view format, Args&&... args)
    {
        std::string message = std::vformat(format, std::make_format_args(args...));
        _Instance()._Enqueue(level, message);
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