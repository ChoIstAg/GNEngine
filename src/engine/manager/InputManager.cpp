#include "engine/manager/InputManager.h"
#include <iostream>

InputManager::InputManager(EventManager& eventManager)
    : eventManager_(eventManager) {

    /* Initialize Key states */
    currentKeyStates_ = SDL_GetKeyboardState(nullptr);

    /* Initialize previous key states */
    for (int i = 0; i < SDL_SCANCODE_COUNT; ++i) {
        previousKeyStates_.reset(i); /* Set false */
    }
}

InputManager::~InputManager() {
    std::cerr << "InputManager " << this << " is successfully destroyed" << std::endl;
}

void InputManager::updateKeyStates() {
    /* Copy previous key state to current. */
    for (int i = 0; i < SDL_SCANCODE_COUNT; ++i) {
        if (currentKeyStates_[i]) {
            previousKeyStates_.set(i);
        } else {
            previousKeyStates_.reset(i);
        }
    }

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

bool InputManager::processEvents() {
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
                if (keyPressTimes_.find(scancode) == keyPressTimes_.end()) { // 새로 눌린 키인지 확인
                    eventManager_.dispatch(KeyPressedEvent(scancode)); /* 이벤트 발생 */
                    keyPressTimes_[scancode] = SDL_GetTicks();
                    currentlyPressedKeys_.insert(scancode);
                }
                break;
            }

            case SDL_EVENT_KEY_UP: {
                SDL_Scancode scancode = event.key.scancode;
                eventManager_.dispatch(KeyReleasedEvent(scancode)); /* 이벤트 발생 */
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