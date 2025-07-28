#include "Input/Mouse.hpp"
#include "Core/Window.hpp"

namespace AE
{
    Mouse::Mouse(Window* window) : _window(window) {}
    
    bool Mouse::IsButtonDown(MouseButton button) const
    {
        return _GetButtonState(button);
    }
    
    bool Mouse::IsButtonUp(MouseButton button) const
    {
        return !_GetButtonState(button);
    }
    
    bool Mouse::IsButtonPressed(MouseButton button) const
    {
        return _GetButtonState(button) && !_GetPrevButtonState(button);
    }
    
    bool Mouse::IsButtonReleased(MouseButton button) const
    {
        return !_GetButtonState(button) && _GetPrevButtonState(button);
    }
    
    const glm::vec2& Mouse::GetPosition() const { return _position; }
    const glm::vec2& Mouse::GetDelta() const { return _delta; }
    const glm::vec2& Mouse::GetScroll() const { return _scroll; }
    
    float Mouse::GetPositionX() const { return _position.x; }
    float Mouse::GetPositionY() const { return _position.y; }
    
    float Mouse::GetDeltaX() const { return _delta.x; }
    float Mouse::GetDeltaY() const { return _delta.y; }
    
    float Mouse::GetScrollX() const { return _scroll.x; }
    float Mouse::GetScrollY() const { return _scroll.y; }
    
    bool Mouse::IsCursorVisible() const { return _cursorVisible; }
    
    void Mouse::SetPosition(const glm::vec2& position, bool global)
    {
        _position = position;
    
        if (global)
            SDL_WarpMouseGlobal(_position.x, _position.y);
        else if (_window && _window->GetWindow())
            SDL_WarpMouseInWindow(_window->GetWindow(), _position.x, _position.y);
    }
    
    void Mouse::SetPositionX(float x, bool global)
    {
        SetPosition({x, _position.y}, global);
    }
    
    void Mouse::SetPositionY(float y, bool global)
    {
        SetPosition({_position.x, y}, global);
    }
    
    void Mouse::SetCursorVisible(bool visible)
    {
        _cursorVisible = visible;
        if (visible) SDL_ShowCursor();
        else SDL_HideCursor();
    }
    
    void Mouse::_HandleEvent(const SDL_Event& event)
    {
        switch (event.type)
        {
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                _SetButtonState(static_cast<MouseButton>(event.button.button), true);
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                _SetButtonState(static_cast<MouseButton>(event.button.button), false);
                break;
            case SDL_EVENT_MOUSE_MOTION:
                _position = glm::vec2(event.motion.x, event.motion.y);
                _delta += glm::vec2(event.motion.xrel, event.motion.yrel);
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                _scroll += glm::vec2(event.wheel.x, event.wheel.y);
                break;
        }
    }
    
    void Mouse::_Update()
    {
        _prevButtonStates = _buttonStates;
        _delta = {};
        _scroll = {};
    }
    
    bool Mouse::_GetButtonState(MouseButton button) const
    {
        return _buttonStates.test(static_cast<size_t>(button));
    }
    
    void Mouse::_SetButtonState(MouseButton button, bool state)
    {
        _buttonStates.set(static_cast<size_t>(button), state);
    }
    
    bool Mouse::_GetPrevButtonState(MouseButton button) const
    {
        return _prevButtonStates.test(static_cast<size_t>(button));
    }
}