#pragma once
#include "../../GNEngine_API.h"

#include "engine/manager/SystemManager.h"

/* 전방선언 */
class RenderManager;
class EntityManager;

/*
 * @brief FadeComponent를 처리하여 화면에 페이드 효과를 렌더링하는 시스템.
 */
class GNEngine_API FadeSystem {
private:
    RenderManager& renderManager_;

public:
    FadeSystem(RenderManager& renderManager);

    /*
     * @brief 매 프레임 호출되어 페이드 상태를 업데이트하고 화면에 그립니다.
     * @param entityManager 엔티티와 컴포넌트를 관리하는 EntityManager.
     * @param deltaTime 이전 프레임과의 시간 간격.
     */
    void update(EntityManager& entityManager, float deltaTime);

    //TODO - 키 입력시 즉시 페이드 시간을 짧게 설정  
};
