#pragma once

#include "engine/manager/EntityManager.h"
#include "engine/manager/EventManager.h"
#include "engine/core/EventInterface.h"
#include "engine/component/AccelerationComponent.h"

/**
 * @class InputToAccelerationSystem
 * @brief ActionEvent를 받아 엔티티의 가속도를 조절하는 시스템.
 * 
 * 이 시스템은 EventManager로부터 ActionEvent를 구독함.
 * "move_"로 시작하는 액션 이벤트를 감지하면, 해당 엔티티의
 * AccelerationComponent를 수정하여 물리적 움직임을 발생시킴.
*/
class InputToAccelerationSystem {
public:
    InputToAccelerationSystem(EventManager& eventManager, EntityManager& entityManager);
    ~InputToAccelerationSystem();

    void update(EntityManager& entityManager);

private:
    EventManager& eventManager_;
    EntityManager& entityManager_;
    EventManager::SubscriptionId actionEventSubId_;

    /**
     * @brief ActionEvent를 처리하는 핸들러 함수.
     * @param event 게임 내 논리적 액션 정보를 담은 이벤트 객체.
     */
    void onActionEvent(const ActionEvent& event);
};