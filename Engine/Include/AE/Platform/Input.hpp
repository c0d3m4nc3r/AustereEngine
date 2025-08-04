#pragma once

#include "PCH.hpp"

#include <SDL3/SDL_events.h>

#include <bitset>

namespace AE
{
    class Window;
    class Keyboard;
    class Mouse;
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

    /* --- Mouse --- */

    enum class MouseButton
    {
        Left = SDL_BUTTON_LEFT,
        Middle = SDL_BUTTON_MIDDLE,
        Right = SDL_BUTTON_RIGHT,
        X1 = SDL_BUTTON_X1,
        X2 = SDL_BUTTON_X2,
    };

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

    /* --- Keyboard --- */

    enum class Key;
    class Keyboard
    {
    public:
    
        Keyboard() = default;
        ~Keyboard() = default;
    
        static Key ScancodeToKey(SDL_Scancode scancode);
        static Key KeycodeToKey(SDL_Keycode keycode);
    
        bool IsKeyPressed(Key key) const;
        bool IsKeyReleased(Key key) const;
        bool IsKeyDown(Key key) const;
        bool IsKeyUp(Key key) const;
    
    private:
    
        std::unordered_map<Key, bool> _keyStates;
        std::unordered_map<Key, bool> _prevKeyStates;
    
        void _HandleEvent(const SDL_Event& event);
        void _Update();
    
        bool _GetKeyState(Key key) const;
        void _SetKeyState(Key key, bool state);
        
        bool _GetPrevKeyState(Key key) const;
    
        friend class InputManager;
    };

    enum class Key
    {
        A = SDL_SCANCODE_A, B = SDL_SCANCODE_B, C = SDL_SCANCODE_C,
        D = SDL_SCANCODE_D, E = SDL_SCANCODE_E, F = SDL_SCANCODE_F,
        G = SDL_SCANCODE_G, H = SDL_SCANCODE_H, I = SDL_SCANCODE_I,
        J = SDL_SCANCODE_J, K = SDL_SCANCODE_K, L = SDL_SCANCODE_L,
        M = SDL_SCANCODE_M, N = SDL_SCANCODE_N, O = SDL_SCANCODE_O,
        P = SDL_SCANCODE_P, Q = SDL_SCANCODE_Q, R = SDL_SCANCODE_R,
        S = SDL_SCANCODE_S, T = SDL_SCANCODE_T, U = SDL_SCANCODE_U,
        V = SDL_SCANCODE_V, W = SDL_SCANCODE_W, X = SDL_SCANCODE_X,
        Y = SDL_SCANCODE_Y, Z = SDL_SCANCODE_Z,
    
        Num0 = SDL_SCANCODE_0, Num1 = SDL_SCANCODE_1, Num2 = SDL_SCANCODE_2,
        Num3 = SDL_SCANCODE_3, Num4 = SDL_SCANCODE_4, Num5 = SDL_SCANCODE_5,
        Num6 = SDL_SCANCODE_6, Num7 = SDL_SCANCODE_7, Num8 = SDL_SCANCODE_8,
        Num9 = SDL_SCANCODE_9,
    
        F1  = SDL_SCANCODE_F1,  F2  = SDL_SCANCODE_F2,  F3  = SDL_SCANCODE_F3,
        F4  = SDL_SCANCODE_F4,  F5  = SDL_SCANCODE_F5,  F6  = SDL_SCANCODE_F6,
        F7  = SDL_SCANCODE_F7,  F8  = SDL_SCANCODE_F8,  F9  = SDL_SCANCODE_F9,
        F10 = SDL_SCANCODE_F10, F11 = SDL_SCANCODE_F11, F12 = SDL_SCANCODE_F12,
    
        LShift = SDL_SCANCODE_LSHIFT, RShift = SDL_SCANCODE_RSHIFT,
        LCtrl  = SDL_SCANCODE_LCTRL,  RCtrl  = SDL_SCANCODE_RCTRL,
        LAlt   = SDL_SCANCODE_LALT,   RAlt   = SDL_SCANCODE_RALT,
        Space  = SDL_SCANCODE_SPACE,
        Enter  = SDL_SCANCODE_RETURN,
    
        Up     = SDL_SCANCODE_UP,
        Down   = SDL_SCANCODE_DOWN,
        Left   = SDL_SCANCODE_LEFT,
        Right  = SDL_SCANCODE_RIGHT,
    
        Home   = SDL_SCANCODE_HOME,
        End    = SDL_SCANCODE_END,
        PageUp = SDL_SCANCODE_PAGEUP,
        PageDown = SDL_SCANCODE_PAGEDOWN,
    
        Insert = SDL_SCANCODE_INSERT,
        Delete = SDL_SCANCODE_DELETE,
    
        Escape = SDL_SCANCODE_ESCAPE,
        Tab    = SDL_SCANCODE_TAB,
        Backspace = SDL_SCANCODE_BACKSPACE,
    
        Numpad0 = SDL_SCANCODE_KP_0, Numpad1 = SDL_SCANCODE_KP_1, Numpad2 = SDL_SCANCODE_KP_2,
        Numpad3 = SDL_SCANCODE_KP_3, Numpad4 = SDL_SCANCODE_KP_4, Numpad5 = SDL_SCANCODE_KP_5,
        Numpad6 = SDL_SCANCODE_KP_6, Numpad7 = SDL_SCANCODE_KP_7, Numpad8 = SDL_SCANCODE_KP_8,
        Numpad9 = SDL_SCANCODE_KP_9, NumpadEnter = SDL_SCANCODE_KP_ENTER,
        NumpadPlus = SDL_SCANCODE_KP_PLUS, NumpadMinus = SDL_SCANCODE_KP_MINUS,
        NumpadMultiply = SDL_SCANCODE_KP_MULTIPLY, NumpadDivide = SDL_SCANCODE_KP_DIVIDE,
    
        NumpadPeriod = SDL_SCANCODE_KP_PERIOD,
        NumpadEquals = SDL_SCANCODE_KP_EQUALS,
    
        PrintScreen = SDL_SCANCODE_PRINTSCREEN,
        ScrollLock  = SDL_SCANCODE_SCROLLLOCK,
        Pause       = SDL_SCANCODE_PAUSE
    };
}
