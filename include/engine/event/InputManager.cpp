#include "InputManager.h"
#include <SDL3/SDL.h>

#include <iostream>
#include <cstring>


#include "InputManager.h"
#include "EventInterface.h" // 이벤트 정의를 위해 포함
#include <SDL3/SDL.h>
#include <iostream>
#include <cstring>

InputManager::InputManager(EventManager& eventManager)
    : eventManager_(eventManager) {}

/* Initialize key states. */
bool InputManager::init() {
    currentKeyStates_ = SDL_GetKeyboardState(nullptr);
    // 이전 키 상태를 현재 상태와 동일하게 초기화하여 첫 프레임에서 오작동 방지
    std::memcpy(previousKeyStates_, currentKeyStates_, SDL_SCANCODE_COUNT);
    return true;
}

void InputManager::updateKeyStates() {
    // 현재 키보드 상태 배열을 이전 상태로 복사
    std::memcpy(previousKeyStates_, currentKeyStates_, SDL_SCANCODE_COUNT);
    eventManager_.dispatch()
    
    // SDL 이벤트 펌프를 호출하여 currentKeyStates_ 배열을 최신 상태로 업데이트
    // SDL_PollEvent 내부에서 자동으로 호출되지만, 명시적으로 호출하여 상태를 정확히 유지
    //SDL_PumpEvents();
    /* 만약 풀링을 eventProcessing에서 진행할 경우 반드시 eventProcessing 뒤에 updateKeyStates를 실행하기 */
}

bool InputManager::eventProcessing() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                eventManager_.dispatch(WindowCloseEvent());
                return false; // 종료 신호 반환
                break;

            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                eventManager_.dispatch(WindowCloseEvent());
                return false; // 종료 신호 반환
                break;

            case SDL_EVENT_WINDOW_RESIZED:
                //std::cout << "Window resized to " << event.window.data1 << "x" << event.window.data2 << std::endl;
                eventManager_.dispatch(WindowResizeEvent(event.window.data1, event.window.data2));
                break;

            case SDL_EVENT_MOUSE_MOTION:
                //std::cout << "Mouse moved to (" << event.motion.x << ", " << event.motion.y << ")" << std::endl;
                eventManager_.dispatch(MouseMovedEvent(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y)));
                break;

            case SDL_EVENT_MOUSE_WHEEL:
                //std::cout << "Mouse scrolled: " << event.wheel.x << ", " << event.wheel.y << std::endl;
                eventManager_.dispatch(MouseScrolledEvent(event.wheel.x, event.wheel.y));
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                //std::cout << "Mouse button pressed: " << event.button.button << std::endl;
                eventManager_.dispatch(MouseButtonPressedEvent(event.button.button));
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                //std::cout << "Mouse button released: " << event.button.button << std::endl;
                eventManager_.dispatch(MouseButtonReleasedEvent(event.button.button));
                break;

            default:
                break;
        }
    }
    return true;
}

bool InputManager::isKeyPressed(SDL_Scancode key) const { /* Return if the key is pressed during the current frame */
    return currentKeyStates_[key];
}

bool InputManager::isKeyDown(SDL_Scancode key) const { /* Return if the key is pressed just now */
    return currentKeyStates_[key] && !previousKeyStates_[key];
}

bool InputManager::isKeyUp(SDL_Scancode key) const { /* Return if the key is released just now */
    return !currentKeyStates_[key] && previousKeyStates_[key];
}