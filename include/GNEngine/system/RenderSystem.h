#pragma once
#include "../../GNEngine_API.h"

#include "GNEngine/manager/RenderManager.h"
#include "GNEngine/manager/EntityManager.h"
#include "GNEngine/manager/TextManager.h"
#include "GNEngine/component/TransformComponent.h"
#include "GNEngine/component/RenderComponent.h"
#include "GNEngine/component/AnimationComponent.h"
#include "GNEngine/component/TextComponent.h"
#include "GNEngine/resource/Texture.h"
#include "GNEngine/component/CameraComponent.h"


/*
 * @class RenderSystem
 * @brief 엔티티의 RenderComponent와 TextComponent를 화면에 렌더링하는 시스템임.
 */

class GNEngine_API RenderSystem {
public:
    RenderSystem(RenderManager& renderManager, TextManager& textManager);

    /*
     * @brief 모든 렌더링 가능한 엔티티를 업데이트하고 그림.
     *        내부적으로 활성화된 카메라를 찾아 뷰포트를 설정함.
     * @param entityManager - 엔티티와 컴포넌트를 관리하는 EntityManager.
     * @param deltaTime - 이 시스템에서는 사용되지 않음.
     */
    void update(EntityManager& entityManager, float deltaTime);

private:
    RenderManager& renderManager_;
    TextManager& textManager_;
};
