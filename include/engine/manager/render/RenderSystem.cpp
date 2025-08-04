#include "RenderSystem.h"
#include <iostream>

RenderSystem::RenderSystem(RenderManager& renderManager)
    : renderManager_(renderManager) {}

void RenderSystem::update(EntityManager& entityManager) {
    for (EntityId entity : entityManager.getEntitiesWith<TransformComponent, RenderComponent>()) {
        TransformComponent* transform = entityManager.getComponent<TransformComponent>(entity);
        RenderComponent* render = entityManager.getComponent<RenderComponent>(entity);
        AnimationComponent* animation = entityManager.getComponent<AnimationComponent>(entity); // 애니메이션 컴포넌트도 가져옴

        Texture* texture = render->getTexture();
        if (!texture) {
            std::cerr << "Error: RenderComponent has null texture. Skipping rendering for entity " << entity << std::endl;
            continue;
        }

        if (render->hasAnimation() && animation) {
            // 애니메이션 컴포넌트가 존재하면 현재 프레임을 렌더링
            const SDL_Rect& srcRect = animation->getCurrentFrameRect();
            renderManager_.renderTexture(texture, transform->positionX_, transform->positionY_, &srcRect);
        } else {
            // 애니메이션 컴포넌트가 없으면 전체 텍스처를 렌더링
            renderManager_.renderTexture(texture, transform->positionX_, transform->positionY_);
        }
    }
}
