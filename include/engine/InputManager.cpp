#include "InputManager.h"
#include <SDL3/SDL.h>

#include "Application.h"
#include <iostream>

#include <cstring>


InputManager::InputManager() {
    initialize();
}

/* Initialize key states. */
bool InputManager::initialize() {
    currentKeyStates = SDL_GetKeyboardState(nullptr);
    std::memset(previousKeyStates, 0, sizeof(previousKeyStates));

    return 1;
}

void InputManager::updateKeys() {
    std::memcpy(previousKeyStates, currentKeyStates, SDL_SCANCODE_COUNT); /* Make currentKeyStates to previousKeyStates*/

    /* Update events. If SDL_PollEvent is called, It is not needed. */
    SDL_PumpEvents();
}

bool InputManager::isKeyPressed(SDL_Scancode key) const { /* Return if the key is pressed just now */
    return currentKeyStates[key];
}

bool InputManager::isKeyDown(SDL_Scancode key) const { /* Return if the key is pressed during the current frame */
    return currentKeyStates[key] && !previousKeyStates[key];
}

bool InputManager::isKeyUp(SDL_Scancode key) const { /* Return if the key is */
    return !currentKeyStates[key] && previousKeyStates[key];
}