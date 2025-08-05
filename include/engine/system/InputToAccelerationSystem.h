#pragma once

#include "engine/manager/EntityManager.h"
#include "engine/component/InputControlComponent.h"
#include "engine/component/AccelerationComponent.h"
#include "config.h"

/*
 * @class InputToAccelerationSystem
 * @brief InputControlComponent의 의도된 입력을 AccelerationComponent로 변환하는 시스템임.
 *        이 시스템은 입력 의도를 기반으로 엔티티에 가속도를 적용함.
*/
class InputToAccelerationSystem {
public:
    InputToAccelerationSystem() = default;

    /*
     * @brief InputControlComponent를 가진 모든 엔티티의 AccelerationComponent를 업데이트함.
     * @param entityManager - 엔티티와 컴포넌트를 관리하는 EntityManager.
     * @param deltaTime - 이전 프레임으로부터 경과된 시간 (초).
     */
    void update(EntityManager& entityManager, float deltaTime);
};