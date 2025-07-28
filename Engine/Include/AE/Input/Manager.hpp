#pragma once

#include "PCH.hpp"

#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"

namespace AE
{
    class Window;
    class InputManager
    {
    public:
    
        InputManager(Window* window = nullptr);
        ~InputManager() = default;
    
        Keyboard* GetKeyboard() const;
        Mouse* GetMouse() const;
    
    private:
    
        std::unique_ptr<Keyboard> _keyboard;
        std::unique_ptr<Mouse> _mouse;
    
        void _HandleEvent(const SDL_Event& event);
        void _Update();
    
        friend class Engine;
    };
}