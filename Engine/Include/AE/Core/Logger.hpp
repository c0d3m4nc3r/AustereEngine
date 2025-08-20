#pragma once

#include "PCH.hpp"

#include <string_view>
#include <stack>
#include <queue>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <fstream>
#include <iostream>
#include <mutex>
#include <vector>

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

        static void PushContext(std::string ctx);
        static void PopContext();

    private:

        Logger();
        ~Logger();

        std::atomic<bool> _running{false};
        std::thread _worker;
        std::mutex _queueMutex;
        std::condition_variable _cv;
        std::queue<std::string> _messageQueue;
        std::vector<std::ostream*> _outputs{ &std::cout };

        std::stack<std::string>& _contextStack()
        {
            thread_local static std::stack<std::string> stack;
            return stack;
        }

        static Logger& _Instance()
        {
            static Logger instance;
            return instance;
        }

        void _Enqueue(LogLevel level, const std::string& msg);

        void _WorkerLoop();

        std::string _GetLevelString(LogLevel level);
        std::string _GetLevelColor(LogLevel level);
        std::string _GetCurrentTimestamp();

        friend class LoggerContext;
    };

    class LoggerContext
    {
    public:
        LoggerContext(const std::string& className, const std::string& methodName);
        ~LoggerContext();

        LoggerContext(const LoggerContext&) = delete;
        LoggerContext& operator=(const LoggerContext&) = delete;

        LoggerContext(LoggerContext&&) noexcept = default;
        LoggerContext& operator=(LoggerContext&&) noexcept = default;
    };
}

#include "Core/Logger.inl"