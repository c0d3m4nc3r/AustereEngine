#include "Platform/Input.hpp"

namespace AE
{
    Key Keyboard::ScancodeToKey(SDL_Scancode scancode)
    {
        return static_cast<Key>(scancode);
    }
    
    Key Keyboard::KeycodeToKey(SDL_Keycode keycode)
    {
        return static_cast<Key>(SDL_GetScancodeFromKey(keycode, NULL));
    }
    
    bool Keyboard::IsKeyPressed(Key key) const
    {
        return _GetKeyState(key) && !_GetPrevKeyState(key);
    }
    
    bool Keyboard::IsKeyReleased(Key key) const
    {
        return !_GetKeyState(key) && _GetPrevKeyState(key);
    }
    
    bool Keyboard::IsKeyDown(Key key) const
    {
        return _GetKeyState(key);
    }
    
    bool Keyboard::IsKeyUp(Key key) const
    {
        return !_GetKeyState(key);
    }
    
    void Keyboard::_HandleEvent(const SDL_Event& event)
    {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            _SetKeyState(ScancodeToKey(event.key.scancode), true);
        } else if (event.type == SDL_EVENT_KEY_UP) {
            _SetKeyState(ScancodeToKey(event.key.scancode), false);
        }
    }
    
    void Keyboard::_Update()
    {
        _prevKeyStates = _keyStates;
    }
    
    bool Keyboard::_GetKeyState(Key key) const
    {
        auto it = _keyStates.find(key);
        return it != _keyStates.end() ? it->second : false;
    }
    
    void Keyboard::_SetKeyState(Key key, bool state)
    {
        _keyStates[key] = state;
    }
    
    bool Keyboard::_GetPrevKeyState(Key key) const
    {
        auto it = _prevKeyStates.find(key);
        return it != _prevKeyStates.end() ? it->second : false;
    }
}