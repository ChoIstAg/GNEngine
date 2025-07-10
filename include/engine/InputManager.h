#pragma once

#include <SDL3/SDL.h>
class Application; /* 순환참조 해결용. Application.h 포함 X*/

class InputManager{
private: 
    const bool* currentKeyStates;
    bool previousKeyStates[SDL_SCANCODE_COUNT];
public:
    InputManager();
    bool initialize();

    void updateKeys();
    bool isKeyPressed(SDL_Scancode key) const;
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyUp(SDL_Scancode key) const;
};