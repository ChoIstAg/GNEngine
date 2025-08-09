#pragma once
#include "../../GNEngine_API.h"

#include "engine/manager/EntityManager.h"
#include "engine/component/CameraComponent.h"
#include "engine/component/TransformComponent.h"
#include "engine/manager/RenderManager.h" // RenderManager.h 추가


/*
 * @class CameraSystem
 * @brief CameraComponent를 가진 엔티티의 카메라 로직을 업데이트하는 시스템임.
 * 주로 타겟 엔티티를 따라가거나 줌 레벨을 조정하는 역할을 함.
 */
class GNEngine_API CameraSystem {
public:
    CameraSystem(RenderManager& renderManager);

    /*
     * @brief 모든 카메라 엔티티의 상태를 업데이트함.
     * @param entityManager - 엔티티와 컴포넌트를 관리하는 EntityManager.
     * @param deltaTime - 이전 프레임으로부터 경과된 시간 (초).
     */
    void update(EntityManager& entityManager, float deltaTime);

private:
    RenderManager& renderManager_;
};
