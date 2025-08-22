#pragma once
#include "../../GNEngine_API.h"

#include "GNEngine/core/Component.h"

/*
 * @brief 오브젝트의 현재 속도를 나타내는 컴포넌트임.
 * @param vx(0.0f) x축 속도.
 * @param vy(0.0f) y축 속도.
*/ 
struct GNEngine_API VelocityComponent : public Component {
    float vx = 0.0f;
    float vy = 0.0f;

    VelocityComponent(float vx = 0.0f, float vy = 0.0f)
        : vx(vx), vy(vy) {}
};

