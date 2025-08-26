#pragma once
#include "../GNEngine_API.h"

#include <vector>

#include "GNEngine/manager/EntityManager.h"
#include "GNEngine/component/AnimationComponent.h"


/*
 * @class AnimationSystem
 * @brief AnimationComponent를 가진 모든 엔티티의 애니메이션 프레임을 업데이트하는 시스템임.
*/
class GNEngine_API AnimationSystem {
public:
    AnimationSystem() = default;

    /*
     * @brief 모든 애니메이션 가능한 엔티티의 애니메이션 프레임을 업데이트함.
     * @param entityManager - 엔티티와 컴포넌트를 관리하는 EntityManager.
     * @param deltaTime - 이전 프레임으로부터 경과된 시간 (초).
     */
    void update(EntityManager& entityManager, float deltaTime);
};




