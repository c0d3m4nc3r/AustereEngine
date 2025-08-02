#include "Platform/Input.hpp"

namespace AE
{
    InputManager::InputManager(Window* window)
    {
        _keyboard = std::make_unique<Keyboard>();
        _mouse = std::make_unique<Mouse>(window);
    }
    
    Keyboard* InputManager::GetKeyboard() const { return _keyboard.get(); }
    Mouse* InputManager::GetMouse() const { return _mouse.get(); }
    
    void InputManager::_HandleEvent(const SDL_Event& event)
    {
        _keyboard->_HandleEvent(event);
        _mouse->_HandleEvent(event);
    }
    
    void InputManager::_Update()
    {
        _keyboard->_Update();
        _mouse->_Update();
    }
}