#pragma once

enum class EventTypes {
    None = 0,
    Quit,
    KeyDown, KeyUp, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved
};

class IKeyboardEventListener {
    public:
    virtual ~IKeyboardEventListener() = default;

}