#include "Core/Window.hpp"
#include "Core/EngineSettings.hpp"
#include "Core/Logger.hpp"

namespace AE
{
    Window::Window(const std::string& title, int width, int height, bool fullscreen, bool resizable, bool borderless, bool vsync)
        : _title(title), _width(width), _height(height),
        _aspectRatio(static_cast<float>(width) / static_cast<float>(height)),
        _fullscreen(fullscreen), _resizable(resizable),
        _borderless(borderless), _vsync(vsync) {}

    Window::~Window()
    {
        if (_state.initialized)
        {
            _Destroy();
        }
    }

    SDL_Window* Window::GetWindow() const { return _window; }
    SDL_GLContext Window::GetGLContext() const { return _glContext; }

    const std::string& Window::GetTitle() const { return _title; }

    void Window::GetPosition(int* x, int* y) const
    {
        if (x) *x = _positionX;
        if (y) *y = _positionY;
    }

    int Window::GetPositionX() const { return _positionX; }
    int Window::GetPositionY() const { return _positionY; }

    void Window::GetSize(int* width, int* height) const
    {
        if (width) *width = _width;
        if (height) *height = _height;
    }

    int Window::GetWidth() const { return _width; }
    int Window::GetHeight() const { return _height; }

    float Window::GetAspectRatio() const { return _aspectRatio; }

    bool Window::IsFullscreen() const { return _fullscreen; }
    bool Window::IsResizable() const { return _resizable; }
    bool Window::IsBorderless() const { return _borderless; }
    bool Window::IsVSyncEnabled() const { return _vsync; }

    bool Window::IsInitialized() const { return _state.initialized; }

    void Window::SetTitle(const std::string& title)
    {
        _title = title;

        SDL_SetWindowTitle(_window, title.c_str());
    }

    void Window::SetPosition(int x, int y)
    {
        _positionX = (x == -1 ? _positionX : x);
        _positionY = (y == -1 ? _positionY : y);

        SDL_SetWindowPosition(_window, _positionX, _positionY);
    }

    void Window::SetSize(int width, int height)
    {
        _width = (width == -1 ? _width : width);
        _height = (height == -1 ? _height : height);

        _aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);

        SDL_SetWindowSize(_window, _width, _height);
    }

    void Window::SetFullscreen(bool fullscreen)
    {
        _fullscreen = fullscreen;

        SDL_SetWindowFullscreen(_window, fullscreen);
    }

    void Window::SetResizable(bool resizable)
    {
        _resizable = resizable;

        SDL_SetWindowResizable(_window, resizable);
    }

    void Window::SetBorderless(bool borderless)
    {
        _borderless = borderless;

        SDL_SetWindowBordered(_window, !borderless);
    }

    void Window::SetVSync(bool enabled)
    {
        _vsync = enabled;

        SDL_GL_SetSwapInterval(enabled ? 1 : 0);
    }

    bool Window::_CreateWindow()
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

        EngineSettings& settings = EngineSettings::Get();

        if (settings.graphics.msaaLevel != MSAALevel::None)
        {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, static_cast<int>(settings.graphics.msaaLevel));
        }
        else
        {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
        }


        SDL_WindowFlags flags = SDL_WINDOW_OPENGL;

        if (_fullscreen) flags |= SDL_WINDOW_FULLSCREEN;
        if (_resizable) flags |= SDL_WINDOW_RESIZABLE;
        if (_borderless) flags |= SDL_WINDOW_BORDERLESS;

        _window = SDL_CreateWindow(_title.c_str(), _width, _height, flags);
        if (!_window)
        {
            Logger::Error("SDL_CreateWindow error: {}", SDL_GetError());
            return false;
        }

        return true;
    }

    bool Window::_CreateGLContext()
    {
        _glContext = SDL_GL_CreateContext(_window);
        if (!_glContext)
        {
            Logger::Error("SDL_GL_CreateContext error: {}", SDL_GetError());
            return false;
        }

        return true;
    }

    bool Window::_InitGLAD()
    {
        if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
        {
            Logger::Error("GLAD initialization failed!");
            return false;
        }

        return true;
    }

    bool Window::_Initialize()
    {
        LoggerContext ctx("Window", "_Initialize");

        Logger::Info("Initializing window...");

        if (_state.initialized)
        {
            Logger::Warning("Window is already initialized! Aborting...");
            return false;
        }

        if (!_CreateWindow() || !_CreateGLContext() || !_InitGLAD())
        {
            Logger::Error("Window initialization failed!");
            return false;
        }

        SDL_GL_SetSwapInterval(_vsync ? 1 : 0);

        _state.initialized = true;

        Logger::Info("Window initialized!");

        return true;
    }

    void Window::_Destroy()
    {
        LoggerContext ctx("Window", "_Destroy");

        Logger::Info("Destroying window...");

        if (!_state.initialized)
        {
            Logger::Warning("Window is not initialized! Aborting...");
            return;
        }

        SDL_GL_DestroyContext(_glContext);
        SDL_DestroyWindow(_window);

        _state.initialized = false;

        Logger::Info("Window destroyed!");
    }

    void Window::_HandleEvent(const SDL_Event& event)
    {
        switch (event.type)
        {
            case SDL_EVENT_WINDOW_RESIZED:
                _width = event.window.data1;
                _height = event.window.data2;
                _aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);
                break;

            case SDL_EVENT_WINDOW_MOVED:
                _positionX = event.window.data1;
                _positionY = event.window.data2;
                break;

            case SDL_EVENT_WINDOW_MINIMIZED:
            case SDL_EVENT_WINDOW_RESTORED:
            case SDL_EVENT_WINDOW_FOCUS_GAINED:
            case SDL_EVENT_WINDOW_FOCUS_LOST:
                break;

            default:
                break;
        }
    }

    void Window::_Update()
    {
        SDL_GL_SwapWindow(_window);
    }
}
