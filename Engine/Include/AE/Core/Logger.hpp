#pragma once

#include "PCH.hpp"

#include <string_view>
#include <stack>

namespace AE
{
    enum class LogLevel { Debug, Info, Warning, Error };

    class LoggerContext;
    class Logger
    {
    public:
        
        template <typename... Args>
        static void Log(LogLevel level, std::string_view format, Args&&... args);

        template <typename... Args>
        static void Debug(std::string_view format, Args&&... args);
        
        template <typename... Args>
        static void Info(std::string_view format, Args&&... args);
        
        template <typename... Args>
        static void Warning(std::string_view format, Args&&... args);
        
        template <typename... Args>
        static void Error(std::string_view format, Args&&... args);

    private:

        static inline std::stack<std::string> _contextStack;

        friend class LoggerContext;
    };

    class LoggerContext
    {
    public:
        LoggerContext(const std::string& className, const std::string& methodName)
        {
            Logger::_contextStack.push(className + "::" + methodName);
        };

        ~LoggerContext()
        {
            Logger::_contextStack.pop();
        }

        LoggerContext(const LoggerContext&) = delete;
        LoggerContext& operator=(const LoggerContext&) = delete;

        LoggerContext(LoggerContext&&) noexcept = default;
        LoggerContext& operator=(LoggerContext&&) noexcept = default;
    };
}

#include "Core/Logger.inl"