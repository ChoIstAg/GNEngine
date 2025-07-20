#pragma once

#include <SDL3/SDL.h>
#include "EventManager.h"

class InputManager{
private: 
    const bool* currentKeyStates_;
    bool previousKeyStates_[SDL_SCANCODE_COUNT];

    EventManager& eventManager_;

public:
    explicit InputManager(EventManager& eventManager);
    bool init();

    void updateKeyStates();

    bool eventProcessing();

    bool isKeyPressed(SDL_Scancode key) const;
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyUp(SDL_Scancode key) const;
};