#pragma once
#include "../../GNEngine_API.h"

#include "engine/manager/RenderManager.h"
#include "engine/manager/EntityManager.h"
#include "engine/component/TransformComponent.h"
#include "engine/component/RenderComponent.h"
#include "engine/component/AnimationComponent.h"
#include "engine/resource/Texture.h"
#include "engine/component/CameraComponent.h"
#include "engine/manager/RenderManager.h"
#include "engine/core/SystemInterface.h"

/*
 * @class RenderSystem
 * @brief TransformComponent와 RenderComponent를 가진 모든 엔티티를 렌더링하는 시스템임.
 */

class GNEngine_API RenderSystem : public SystemInterface {
public:
    RenderSystem(RenderManager& renderManager);

    /*
     * @brief 모든 렌더링 가능한 엔티티를 업데이트하고 그림.
     *        내부적으로 활성화된 카메라를 찾아 뷰포트를 설정함.
     * @param entityManager - 엔티티와 컴포넌트를 관리하는 EntityManager.
     * @param deltaTime - 이 시스템에서는 사용되지 않음.
     */
    void update(EntityManager& entityManager, float deltaTime) override;

private:
    RenderManager& renderManager_;
};