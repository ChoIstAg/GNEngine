#pragma once
#include "../GNEngine_API.h"

#include "GNEngine/core/Component.h"

#include <SDL3/SDL_scancode.h>
#include <map>
#include <string>

/*
 * @class InputControlComponent
 * @brief 엔티티의 키 입력을 특정 액션에 매핑하는 컴포넌트임.
 *        InputSystem은 이 컴포넌트를 참조하여 키 입력이 발생했을 때
 *        해당 키에 매핑된 액션 이벤트를 발생시킴.
 * @param keyActions 키(SDL_Scancode)와 액션 이름(std::string)을 매핑하는 맵.
*/ 
struct GNEngine_API InputControlComponent : public Component {
    std::unordered_map<SDL_Scancode, std::string> keyActions;
};




