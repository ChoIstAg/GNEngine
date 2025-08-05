#pragma once

#include "engine/core/Component.h"

/*
 * @class InputControlComponent
 * @brief 입력에 의해 제어되는 엔티티의 의도된 움직임 입력을 나타내는 컴포넌트임.
 *        이 컴포넌트는 엔티티가 어떤 방향으로 움직이려 하는지, 어떤 액션을 취하려 하는지 등의
 *        입력 상태를 저장함. 실제 움직임이나 액션은 다른 시스템에서 이 컴포넌트를 참조하여 처리함.
*/
struct InputControlComponent : public Component {
    float desiredX = 0.0f; // -1.0 (left), 0.0 (none), 1.0 (right)
    float desiredY = 0.0f; // -1.0 (up), 0.0 (none), 1.0 (down)
};