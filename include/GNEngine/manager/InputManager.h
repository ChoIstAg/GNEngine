#pragma once
#include "../GNEngine_API.h"

#include <SDL3/SDL.h>
#include <set>
#include <unordered_map>
#include <vector>
#include <bitset>

#include "EventManager.h"
#include "GNEngine/core/EventInterface.h"

class GNEngine_API InputManager {
private: 
    const bool* currentKeyStates_;
    std::bitset<SDL_SCANCODE_COUNT> previousKeyStates_; // Changed to std::bitset
    std::set<SDL_Scancode> currentlyPressedKeys_; /* 현재 눌린 키 목록 */
    std::unordered_map<SDL_Scancode, uint32_t> keyPressTimes_; /* 키와 SDL_EVENT_KEY_DOWN이 일어났을 때부터 측정한 시간*/

    EventManager& eventManager_;

public:
    explicit InputManager(EventManager& eventManager);
    ~InputManager();

    bool processEvents(); /* 모든 SDL 이벤트를 폴링하고 처리함*/
    void updateKeyStates(); /* 모든 키 상태를 업데이트함*/

    bool isKeyPressed(SDL_Scancode key) const;
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyUp(SDL_Scancode key) const;
};



