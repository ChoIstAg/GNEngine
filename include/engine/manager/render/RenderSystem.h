#pragma once

#include "engine/manager/render/RenderManager.h"
#include "engine/ecs/EntityManager.h"
#include "engine/component/TransformComponent.h"
#include "engine/component/RenderComponent.h"
#include "engine/component/AnimationComponent.h"
#include "engine/resource/texture/Texture.h"

/*
 * @class RenderSystem
 * @brief TransformComponent와 RenderComponent를 가진 모든 엔티티를 렌더링하는 시스템임.
*/
class RenderSystem {
public:
    RenderSystem(RenderManager& renderManager);

    /*
     * @brief 모든 렌더링 가능한 엔티티를 업데이트하고 그림.
     * @param entityManager - 엔티티와 컴포넌트를 관리하는 EntityManager.
     */
    void update(EntityManager& entityManager);

private:
    RenderManager& renderManager_;
};
