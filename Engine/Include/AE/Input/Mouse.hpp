#pragma once

#include "PCH.hpp"

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_events.h>

#include <bitset>

namespace AE
{
    enum class MouseButton
    {
        Left = SDL_BUTTON_LEFT,
        Middle = SDL_BUTTON_MIDDLE,
        Right = SDL_BUTTON_RIGHT,
        X1 = SDL_BUTTON_X1,
        X2 = SDL_BUTTON_X2,
    };
    
    class Window;
    class Mouse
    {
    public:
    
        Mouse(Window* window = nullptr);
        ~Mouse() = default;
    
        bool IsButtonDown(MouseButton button) const;
        bool IsButtonUp(MouseButton button) const;
        bool IsButtonPressed(MouseButton button) const;
        bool IsButtonReleased(MouseButton button) const;
    
        const glm::vec2& GetPosition() const;
        const glm::vec2& GetDelta() const;
        const glm::vec2& GetScroll() const;
    
        float GetPositionX() const;
        float GetPositionY() const;
        
        float GetDeltaX() const;
        float GetDeltaY() const;
    
        float GetScrollX() const;
        float GetScrollY() const;
    
        bool IsCursorVisible() const;
    
        void SetPosition(const glm::vec2& position, bool global = false);
        void SetPositionX(float x, bool global = false);
        void SetPositionY(float y, bool global = false);
    
        void SetCursorVisible(bool visible);
    
    private:
    
        Window* _window = nullptr;
    
        std::bitset<8> _buttonStates;
        std::bitset<8> _prevButtonStates;
    
        glm::vec2 _position, _delta, _scroll;
    
        bool _cursorVisible = true;
    
        void _HandleEvent(const SDL_Event& event);
        void _Update();
    
        bool _GetButtonState(MouseButton button) const;
        bool _GetPrevButtonState(MouseButton button) const;
    
        void _SetButtonState(MouseButton button, bool state);
    
        friend class InputManager;
    };
}