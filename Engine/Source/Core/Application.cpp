#include "Core/Application.hpp"
#include "Core/Logger.hpp"

namespace AE
{
    Application::Application(const ApplicationInfo& info)
        : _info(info) {}

    Application::~Application()
    {
        if (_state.initialized)
        {
            LoggerContext ctx("Application", "~Application");
            Logger::Warning("Destructor called before shutdown!");
            _Shutdown();
        }
    }

    const ApplicationInfo& Application::GetInfo() const { return _info; }
    const std::string& Application::GetName() const { return _info.name; }
    const std::string& Application::GetVersion() const { return _info.version; }
    const std::string& Application::GetAuthor() const { return _info.author; }
    const std::string& Application::GetDescription() const { return _info.description; }

    bool Application::IsInitialized() const { return _state.initialized; }

    bool Application::_Initialize()
    {
        LoggerContext ctx("Application", "_Initialize");

        Logger::Info("Initializing application...");

        if (_state.initialized)
        {
            Logger::Warning("Application is already initialized! Aborting...");
            return false;
        }
        
        if (!OnInitialize())
        {
            Logger::Error("Application initialization failed!");
            return false;
        }

        _state.initialized = true;

        Logger::Info("Application initialized!");

        return true;
    }

    void Application::_Shutdown()
    {
        LoggerContext ctx("Application", "_Shutdown");

        Logger::Info("Shutting down application...");

        if (!_state.initialized)
        {
            Logger::Warning("Application is not initialized! Aborting...");
            return;
        }

        OnShutdown();

        _state.initialized = false;

        Logger::Info("Application shut down!");
    }

    void Application::_Update()
    {
        LoggerContext ctx("Application", "_Update");

        if (_state.initialized)
            OnUpdate();
    }

    void Application::_Render()
    {
        LoggerContext ctx("Application", "_Render");

        if (_state.initialized)
            OnRender();
    }

    void Application::_SetEngine(Engine* engine)
    {
        this->engine = engine;
    }

}