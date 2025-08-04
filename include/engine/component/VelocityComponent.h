#pragma once
#include "Component.h"

/*
 * @brief 오브젝트의 현재 속도를 나타내는 컴포넌트임.
*/
struct VelocityComponent : public Component {
    float vx = 0.0f;
    float vy = 0.0f;

    VelocityComponent(float vx = 0.0f, float vy = 0.0f)
        : vx(vx), vy(vy) {}
};
