#include "InputManager.h"
#include "EventInterface.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <cstring>
#include <vector>

InputManager::InputManager(EventManager& eventManager)
    : eventManager_(eventManager) {}

/* Initialize key states. */
bool InputManager::init() {
    currentKeyStates_ = SDL_GetKeyboardState(nullptr);
    std::memcpy(previousKeyStates_, currentKeyStates_, SDL_SCANCODE_COUNT);
    return true;
}

void InputManager::updateKeyStates() {
    std::memcpy(previousKeyStates_, currentKeyStates_, SDL_SCANCODE_COUNT);

    if (!currentlyPressedKeys_.empty()) {
        std::vector<KeyHeldInfo> heldKeysInfo;
        uint32_t currentTime = SDL_GetTicks();

        for (const auto& scancode : currentlyPressedKeys_) {
            uint32_t duration = 0;
            auto it = keyPressTimes_.find(scancode);
            if (it != keyPressTimes_.end()) {
                duration = currentTime - it->second;
            }
            heldKeysInfo.push_back({scancode, duration});
        }
        eventManager_.dispatch(KeysHeldEvent(heldKeysInfo));
    }
}

bool InputManager::eventProcessing() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                eventManager_.dispatch(WindowCloseEvent());
                return false;

            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                eventManager_.dispatch(WindowCloseEvent());
                return false;
            
            case SDL_EVENT_KEY_DOWN: {
                SDL_Scancode scancode = event.key.scancode;
                if (keyPressTimes_.find(scancode) == keyPressTimes_.end()) { // Check if it's a new press
                    eventManager_.dispatch(KeyPressedEvent(scancode));
                    keyPressTimes_[scancode] = SDL_GetTicks();
                    currentlyPressedKeys_.insert(scancode);
                }
                break;
            }

            case SDL_EVENT_KEY_UP: {
                SDL_Scancode scancode = event.key.scancode;
                eventManager_.dispatch(KeyReleasedEvent(scancode));
                currentlyPressedKeys_.erase(scancode);
                keyPressTimes_.erase(scancode);
                break;
            }

            case SDL_EVENT_WINDOW_RESIZED:
                eventManager_.dispatch(WindowResizeEvent(event.window.data1, event.window.data2));
                break;

            case SDL_EVENT_MOUSE_MOTION:
                eventManager_.dispatch(MouseMovedEvent(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y)));
                break;

            case SDL_EVENT_MOUSE_WHEEL:
                eventManager_.dispatch(MouseScrolledEvent(event.wheel.x, event.wheel.y));
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                eventManager_.dispatch(MouseButtonPressedEvent(event.button.button));
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                eventManager_.dispatch(MouseButtonReleasedEvent(event.button.button));
                break;

            default:
                break;
        }
    }
    return true;
}

bool InputManager::isKeyPressed(SDL_Scancode key) const {
    return currentKeyStates_[key];
}

bool InputManager::isKeyDown(SDL_Scancode key) const {
    return currentKeyStates_[key] && !previousKeyStates_[key];
}

bool InputManager::isKeyUp(SDL_Scancode key) const {
    return !currentKeyStates_[key] && previousKeyStates_[key];
}