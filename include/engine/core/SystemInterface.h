#pragma once
#include "../../GNEngine_API.h"

#include "engine/manager/EntityManager.h"

/**
 * @class SystemInterface
 * @brief 모든 시스템 클래스의 기반이 되는 순수 가상 인터페이스.
 * 
 * 이 인터페이스는 시스템들이 공통적으로 가져야 할 함수(예: update)를 정의하여
 * SystemManager가 모든 시스템을 일관된 방식으로 관리할 수 있도록 함.
 */ 
class GNEngine_API SystemInterface {
public:
    virtual ~SystemInterface() = default;

    /**
     * @brief 시스템의 로직을 매 프레임 업데이트하는 순수 가상 함수.
     * @param entityManager 엔티티 및 컴포넌트에 접근하기 위한 EntityManager.
     * @param deltaTime 이전 프레임과의 시간 간격.
     */
    virtual void update(EntityManager& entityManager, float deltaTime) = 0;
};