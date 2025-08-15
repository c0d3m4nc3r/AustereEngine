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
        void SetApplication(UPtr<Application>&& application);

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

        UPtr<Application> _application;
        UPtr<Window> _window;
        UPtr<Renderer> _renderer;
        UPtr<InputManager> _inputMgr;
        UPtr<SceneManager> _sceneMgr;
        UPtr<LightManager> _lightMgr;
        UPtr<ShaderManager> _shaderMgr;
        UPtr<TextureManager> _textureMgr;
        UPtr<CubemapManager> _cubemapMgr;
        UPtr<ModelManager> _modelMgr;

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
