#include "Core/Logger.hpp"

namespace AE
{
    LoggerContext::LoggerContext(const std::string& className, const std::string& methodName)
    {
        Logger::PushContext(className + "::" + methodName);
    }

    LoggerContext::~LoggerContext()
    {
        Logger::PopContext();
    }
}
