#pragma once

#include "engine/manager/EntityManager.h"
#include "engine/manager/EventManager.h"
#include "engine/core/EventInterface.h"

/**
 * @class InputSystem
 * @brief 키 입력을 감지하여 의미있는 액션 이벤트(ActionEvent)를 발생시키는 시스템.
 * 
 * 이 시스템은 EventManager로부터 KeysHeldEvent를 구독함.
 * 이벤트가 발생하면, InputControlComponent를 가진 엔티티들을 확인하여
 * 눌린 키에 매핑된 액션이 있는지 찾고, 있다면 해당 액션에 대한 ActionEvent를 발생시킴.
 * 이 시스템은 게임 로직을 직접 처리하지 않고, 단지 입력을 해석하여 시스템 전체에 알리는 역할만 함.
*/
class InputSystem {
public:
    InputSystem(EventManager& eventManager, EntityManager& entityManager);
    ~InputSystem();

    /**
     * @brief 이 시스템은 이벤트 기반으로만 작동하므로 update 함수가 필요 없음.
     *        대신 onKeysHeld와 같은 이벤트 핸들러가 로직을 처리함.
     */

private:
    EventManager& eventManager_;
    EventManager::SubscriptionId keysHeldSubId_;

    /**
     * @brief KeysHeldEvent를 처리하는 핸들러 함수.
     * @param event 현재 프레임에 눌려있는 키들의 정보를 담은 이벤트 객체.
     * @param entityManager 엔티티와 컴포넌트에 접근하기 위한 EntityManager.
     */
    void onKeysHeld(const KeysHeldEvent& event, EntityManager& entityManager);
};