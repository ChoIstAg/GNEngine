#pragma once

#include <SDL3/SDL.h>
#include <set>
#include <unordered_map>
#include "EventManager.h"

class InputManager {
private: 
    const bool* currentKeyStates_;
    bool previousKeyStates_[SDL_SCANCODE_COUNT];
    std::set<SDL_Scancode> currentlyPressedKeys_; /* 현재 눌린 키 목록 */
    std::unordered_map<SDL_Scancode, uint32_t> keyPressTimes_; /* 키와 SDL_EVENT_KEY_DOWN이 일어났을 때부터 측정한 시간*/

    EventManager& eventManager_;

public:
    explicit InputManager(EventManager& eventManager);
    bool init();

    bool eventProcessing();
    
    void updateKeyStates();

    bool isKeyPressed(SDL_Scancode key) const;
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyUp(SDL_Scancode key) const;
};