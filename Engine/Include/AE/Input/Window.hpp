#pragma once

#include "PCH.hpp"

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>

namespace AE
{
    class Window
    {
    public:

        Window(
            const std::string& title = "Austere Engine",
            int width = 800,
            int height = 600,
            bool fullscreen = false,
            bool resizable = true,
            bool borderless = false,
            bool vsync = true
        );

        ~Window();

        SDL_Window* GetWindow() const;
        SDL_GLContext GetGLContext() const;
        
        const std::string& GetTitle() const;
        
        void GetSize(int* width, int* height) const;
        int GetWidth() const;
        int GetHeight() const;
        float GetAspectRatio() const;
        
        void GetPosition(int* x, int* y) const;
        int GetPositionX() const;
        int GetPositionY() const;

        bool IsFullscreen() const;
        bool IsResizable() const;
        bool IsBorderless() const;
        bool IsVSyncEnabled() const;

        bool IsInitialized() const;

        void SetTitle(const std::string& title);
        void SetPosition(int x, int y);
        void SetSize(int width, int height);

        void SetFullscreen(bool fullscreen);
        void SetResizable(bool resizable);
        void SetBorderless(bool borderless);
        void SetVSync(bool enabled);

    private:

        SDL_Window* _window = nullptr;
        SDL_GLContext _glContext = nullptr;

        std::string _title;
        int _positionX, _positionY;
        int _width, _height;
        float _aspectRatio;

        bool _fullscreen;
        bool _resizable;
        bool _borderless;
        bool _vsync;

        struct WindowState
        {
            bool initialized = false;
        } _state;
        
        bool _CreateWindow();
        bool _CreateGLContext();
        bool _InitGLAD();
        
        bool _Initialize();
        void _Destroy();
        
        void _HandleEvent(const SDL_Event& event);
        
        void _Update();

        friend class Engine;
    };
}
