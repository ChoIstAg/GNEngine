#pragma once

#include "engine/ecs/EntityManager.h"
#include "engine/component/TransformComponent.h"
#include "engine/component/VelocityComponent.h"
#include "engine/component/AccelerationComponent.h"
#include <vector>

/*
 * @class MovementSystem
 * @brief TransformComponent, VelocityComponent, AccelerationComponent를 가진 엔티티들의 위치를 업데이트하는 시스템임.
 *        가속도와 감속도를 적용하여 부드러운 움직임을 구현함.
*/
class MovementSystem {
public:
    MovementSystem() = default;

    /*
     * @brief 모든 움직임 가능한 엔티티의 위치를 업데이트함.
     * @param entityManager - 엔티티와 컴포넌트를 관리하는 EntityManager.
     * @param deltaTime - 이전 프레임으로부터 경과된 시간 (초).
     */
    void update(EntityManager& entityManager, float deltaTime);
};
