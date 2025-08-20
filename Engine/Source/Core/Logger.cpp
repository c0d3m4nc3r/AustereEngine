#include "Core/Logger.hpp"

#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

namespace AE
{
    Logger::Logger()
    {
        _running = true;
        _worker = std::thread(&Logger::_WorkerLoop, this);
    }

    Logger::~Logger()
    {
        {
            std::lock_guard<std::mutex> lock(_queueMutex);
            _running = false;
        }
        _cv.notify_all();
        if (_worker.joinable())
            _worker.join();
    }

    void Logger::PushContext(std::string ctx)
    {
        _Instance()._contextStack().push(std::move(ctx));
    }

    void Logger::PopContext()
    {
        auto& stack = _Instance()._contextStack();
        if (!stack.empty()) stack.pop();
    }

    void Logger::_Enqueue(LogLevel level, const std::string& msg)
    {
        std::ostringstream oss;
        std::string currContext = _contextStack().empty() ? "" : _contextStack().top();

        oss << _GetLevelColor(level) << _GetCurrentTimestamp() << " | [" 
            << _GetLevelString(level) << "] [" << currContext << "] > " 
            << msg << "\033[0m";

        {
            std::lock_guard<std::mutex> lock(_queueMutex);
            _messageQueue.push(oss.str());
        }
        
        _cv.notify_one();
    }

    void Logger::_WorkerLoop()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _cv.wait(lock, [this] { return !_messageQueue.empty() || !_running; });

            if (!_running && _messageQueue.empty())
                break;

            while (!_messageQueue.empty())
            {
                auto msg = std::move(_messageQueue.front());
                _messageQueue.pop();
                
                lock.unlock();

                for (auto* out : _outputs)
                {
                    (*out) << msg << std::endl;
                }
                
                lock.lock();
            }
        }
    }
    
    std::string Logger::_GetLevelString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARN";
        case LogLevel::Error: return "ERROR";
        default: return "UNKNOWN";
        }
    }

    std::string Logger::_GetLevelColor(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Debug: return "\033[36m";
        case LogLevel::Info: return "\033[32m";
        case LogLevel::Warning: return "\033[33m";
        case LogLevel::Error: return "\033[31m";
        default: return "\033[0m";
        }
    }

    std::string Logger::_GetCurrentTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);

        std::tm tm{};
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&tm, &time);
    #else
        localtime_r(&time, &tm);
    #endif

        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
    }
}