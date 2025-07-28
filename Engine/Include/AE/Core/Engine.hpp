#pragma once

#include "PCH.hpp"

namespace AE
{
    class Application;
    class Window;
    class Renderer;
    class InputManager;
    class SceneManager;
    class LightManager;
    class ShaderManager;
    class TextureManager;
    class CubemapManager;
    class ModelManager;
    class Engine
    {
    public:
        
        Engine();
        ~Engine();
        
        int Run(int argc, char* argv[]);
        void Stop();

        Application* GetApplication() const;
        void SetApplication(std::unique_ptr<Application>&& application);

        Window* GetWindow() const;
        Renderer* GetRenderer() const;
        InputManager* GetInputManager() const;
        SceneManager* GetSceneManager() const;
        LightManager* GetLightManager() const;
        ShaderManager* GetShaderManager() const;
        TextureManager* GetTextureManager() const;
        CubemapManager* GetCubemapManager() const;
        ModelManager* GetModelManager() const;

        int GetFPS() const;
        float GetDeltaTime() const;

        bool IsInitialized() const;
        bool IsRunning() const;

    private:

        std::unique_ptr<Application> _application;
        std::unique_ptr<Window> _window;
        std::unique_ptr<Renderer> _renderer;
        std::unique_ptr<InputManager> _inputMgr;
        std::unique_ptr<SceneManager> _sceneMgr;
        std::unique_ptr<LightManager> _lightMgr;
        std::unique_ptr<ShaderManager> _shaderMgr;
        std::unique_ptr<TextureManager> _textureMgr;
        std::unique_ptr<CubemapManager> _cubemapMgr;
        std::unique_ptr<ModelManager> _modelMgr;

        uint32_t _frameCount = 0;
        uint32_t _framesPerSecond = 0;
        
        uint64_t _lastFrameTime = 0;
        uint64_t _lastFpsTime = 0;

        float _deltaTime = 0.0f;
        
        struct EngineState {
            bool initialized = false;
            bool running = false;      
        } _state;

        bool _Initialize();
        void _Shutdown();
        void _PollEvents();
        void _Update();
        void _Render();
    };
}
