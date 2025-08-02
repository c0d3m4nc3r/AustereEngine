#include "Core/Engine.hpp"
#include "Core/EngineSettings.hpp"
#include "Core/Application.hpp"
#include "Core/Logger.hpp"
#include "Platform/Input.hpp"
#include "Platform/Window.hpp"
#include "Lighting/Manager.hpp"
#include "Rendering/Renderer.hpp"
#include "Resources/Managers.hpp"
#include "Scene/Manager.hpp"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>

namespace AE
{
    Engine::Engine() {}

    Engine::~Engine()
    {
        if (_state.initialized)
        {
            LoggerContext ctx("Engine", "~Engine");
            Logger::Warning("Destructor called before shutdown!");
            _Shutdown();
        }
    }

    int Engine::Run(int argc, char* argv[])
    {
        LoggerContext ctx("Engine", "Run");

        Logger::Info("Starting engine...");

        if (!_application)
        {
            Logger::Error("Application is not set! Aborting...");
            return -1;
        }

        if (!_Initialize())
        {
            Logger::Error("Engine initialization failed! Aborting...");
            return -2;
        }

        if (!_application->_Initialize())
        {
            Logger::Error("Application initialization failed! Aborting...");
            _Shutdown();
            return -3;
        }

        Logger::Info("Engine started!");

        _state.running = true;
        while (_state.running)
        {
            _Update();
            _Render();
        }

        _application->_Shutdown();

        _Shutdown();

        Logger::Info("Engine stopped!");

        return 0;
    }

    void Engine::Stop()
    {
        LoggerContext ctx("Engine", "Stop");

        Logger::Info("Stopping engine...");

        _state.running = false;
    }

    Application* Engine::GetApplication() const { return _application.get(); }
    void Engine::SetApplication(std::unique_ptr<Application>&& application)
    {
        _application = std::move(application);
        if (_application)
            _application->_SetEngine(this);
    }

    Window* Engine::GetWindow() const { return _window.get(); }
    Renderer* Engine::GetRenderer() const { return _renderer.get(); }
    InputManager* Engine::GetInputManager() const { return _inputMgr.get(); }
    SceneManager* Engine::GetSceneManager() const { return _sceneMgr.get(); }
    LightManager* Engine::GetLightManager() const { return _lightMgr.get(); }
    ShaderManager* Engine::GetShaderManager() const { return _shaderMgr.get(); }
    TextureManager* Engine::GetTextureManager() const { return _textureMgr.get(); }
    CubemapManager* Engine::GetCubemapManager() const { return _cubemapMgr.get(); }
    ModelManager* Engine::GetModelManager() const { return _modelMgr.get(); }

    int Engine::GetFPS() const { return _framesPerSecond; }
    float Engine::GetDeltaTime() const { return _deltaTime; }

    bool Engine::IsInitialized() const { return _state.initialized; }
    bool Engine::IsRunning() const { return _state.running; }

    bool Engine::_Initialize()
    {
        LoggerContext ctx("Engine", "_Initialize");

        Logger::Info("Initializing engine...");

        if (_state.initialized)
        {
            Logger::Warning("Engine is already initialized! Aborting...");
            return false;
        }

        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            Logger::Error("Failed to initialize SDL: {}", SDL_GetError());
            return false;
        }

        EngineSettings& settings = EngineSettings::Get();

        _window = std::make_unique<Window>(
            settings.window.title,
            settings.window.width,
            settings.window.height,
            settings.window.fullscreen,
            settings.window.resizable,
            settings.window.borderless,
            settings.window.vsync
        );
        if (!_window->_Initialize())
        {
            Logger::Error("Window initialization failed! Aborting...");
            return false;
        }

        _lightMgr = std::make_unique<LightManager>();

        _renderer = std::make_unique<Renderer>(_lightMgr.get());
        if (!_renderer)
        {
            Logger::Error("Renderer initialization failed! Aborting...");
            return false;
        }

        _inputMgr = std::make_unique<InputManager>(_window.get());
        _sceneMgr = std::make_unique<SceneManager>(this);

        _shaderMgr = std::make_unique<ShaderManager>();
        _textureMgr = std::make_unique<TextureManager>();
        _cubemapMgr = std::make_unique<CubemapManager>();
        _modelMgr = std::make_unique<ModelManager>(_textureMgr.get());
        
        _state.initialized = true;
        
        Logger::Info("Engine initialized!");
        
        return true;
    }

    void Engine::_Shutdown()
    {
        LoggerContext ctx("Engine", "_Shutdown");

        Logger::Info("Shutting down engine...");

        if (!_state.initialized)
        {
            Logger::Warning("Engine is not initialized! Aborting...");
            return;
        }
        
        _inputMgr.reset();
        _sceneMgr.reset();
        
        _shaderMgr.reset();
        _textureMgr.reset();
        _cubemapMgr.reset();
        _modelMgr.reset();

        _lightMgr.reset();
        
        _renderer->_Shutdown();
        _renderer.reset();
        
        _window->_Destroy();
        _window.reset();
        
        SDL_Quit();

        _state.initialized = false;
    
        Logger::Info("Engine shut down!");
    }

    void Engine::_PollEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                Stop();

            _window->_HandleEvent(event);
            _inputMgr->_HandleEvent(event);
        }
    }

    void Engine::_Update()
    {
        uint64_t now = SDL_GetPerformanceCounter();
        _deltaTime = static_cast<double>(now - _lastFrameTime) / SDL_GetPerformanceFrequency();
        _lastFrameTime = now;

        if (++_frameCount, now - _lastFpsTime >= SDL_GetPerformanceFrequency())
        {
            _framesPerSecond = _frameCount;
            _frameCount = 0;
            _lastFpsTime = now;
        }

        _inputMgr->_Update();
        _PollEvents();
        _application->_Update();
        _sceneMgr->_Update();
    }

    void Engine::_Render()
    {
        _renderer->_PrepareFrame();

        _application->_Render();
        _sceneMgr->_Render();
   
        _renderer->_RenderFrame();
        
        _window->_Update();
    }
}
