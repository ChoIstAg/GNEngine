#pragma once
#include "../../GNEngine_API.h"

#include "engine/manager/EntityManager.h"
#include "engine/manager/EventManager.h"
#include "engine/core/EventInterface.h"
#include "engine/component/AccelerationComponent.h"
#include "engine/core/SystemInterface.h" // SystemInterface 추가

/**
 * @class InputToAccelerationSystem
 * @brief ActionEvent를 받아 엔티티의 가속도를 조절하는 시스템.
 * 
 * 이 시스템은 EventManager로부터 ActionEvent를 구독함.
 * "move_"로 시작하는 액션 이벤트를 감지하면, 해당 엔티티의
 * AccelerationComponent를 수정하여 물리적 움직임을 발생시킴.
*/
class GNEngine_API InputToAccelerationSystem : public SystemInterface { // SystemInterface 상속
public:
    InputToAccelerationSystem(EventManager& eventManager, EntityManager& entityManager);
    ~InputToAccelerationSystem();

    /**
     * @brief 매 프레임 입력으로 인한 가속도를 0으로 초기화함.
     */
    void update(EntityManager& entityManager, float deltaTime) override;

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