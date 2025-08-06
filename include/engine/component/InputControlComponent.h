#pragma once

#include "engine/core/Component.h"

/*
 * @class InputControlComponent
 * @brief 입력에 의해 제어되는 엔티티의 의도된 움직임 입력을 나타내는 컴포넌트임.
 *        이 컴포넌트는 엔티티가 어떤 방향으로 움직이려 하는지, 어떤 액션을 취하려 하는지 등의
 *        입력 상태를 저장함. 실제 움직임이나 액션은 다른 시스템에서 이 컴포넌트를 참조하여 처리함.
*/
#include <map>
#include <string>
#include <SDL3/SDL_scancode.h>

/*
 * @class InputControlComponent
 * @brief 엔티티의 키 입력을 특정 액션에 매핑하는 컴포넌트임.
 *        InputSystem은 이 컴포넌트를 참조하여 키 입력이 발생했을 때
 *        해당 키에 매핑된 액션 이벤트를 발생시킴.
 * 
 * @field keyActions 키(SDL_Scancode)와 액션 이름(std::string)을 매핑하는 맵.
*/
struct InputControlComponent : public Component {
    std::map<SDL_Scancode, std::string> keyActions;
};