#pragma once

#include "engine/manager/EntityManager.h"
#include "engine/manager/EventManager.h"
#include "engine/component/PlayerMovementComponent.h"
#include "engine/component/VelocityComponent.h"
#include "engine/component/AccelerationComponent.h"
#include "engine/core/EventInterface.h"
#include <iostream>
#include <cmath>
#include <algorithm>

/*
 * @class InputSystem
 * @brief InputManager로부터 이벤트를 받아 PlayerMovementComponent를 가진 엔티티의
 *        VelocityComponent와 AccelerationComponent를 업데이트하는 시스템임.
*/
class InputSystem {
public:
    InputSystem(EventManager& eventManager, EntityManager& entityManager);
    ~InputSystem();

    void update(EntityManager& entityManager, float deltaTime);

private:
    EventManager& eventManager_;
    EntityManager& entityManager_; // Add EntityManager as a member
    EventManager::SubscriptionId keysHeldSubId_;

    // 이동 속도 및 가속도 관련 상수
    // const float MOVE_SPEED = 100.0f; // 초당 이동 속도
    // const float ACCELERATION_RATE = 500.0f; // 초당 가속도
    // const float DECELERATION_RATE = 500.0f; // 초당 감속도

    void onKeysHeld(const KeysHeldEvent& event);
};