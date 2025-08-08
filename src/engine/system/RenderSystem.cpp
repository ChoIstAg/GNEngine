#include "engine/system/RenderSystem.h"
#include <iostream>

RenderSystem::RenderSystem(RenderManager& renderManager)
    : renderManager_(renderManager) {}

void RenderSystem::update(EntityManager& entityManager, float deltaTime) {
    SDL_Log("RenderSystem: update called.");
    // 렌더링할 모든 엔티티 순회
    for (EntityId entity : entityManager.getEntitiesWith<RenderComponent, TransformComponent>()) {
        RenderComponent* renderComponent = entityManager.getComponent<RenderComponent>(entity);
        TransformComponent* transformComponent = entityManager.getComponent<TransformComponent>(entity);

        SDL_Log("RenderSystem: Rendering entity %u at (%.2f, %.2f)", entity, transformComponent->positionX_, transformComponent->positionY_);

        if (renderComponent->getTexture()) {
            SDL_Rect srcRect = renderComponent->getSrcRect();
            float destX = transformComponent->positionX_;
            float destY = transformComponent->positionY_;
            float destW = static_cast<float>(srcRect.w);
            float destH = static_cast<float>(srcRect.h);

            // 애니메이션 컴포넌트가 있으면 현재 프레임 사용
            if (renderComponent->hasAnimation() && entityManager.hasComponent<AnimationComponent>(entity)) {
                AnimationComponent* animationComponent = entityManager.getComponent<AnimationComponent>(entity);
                if (animationComponent->getAnimation()) {
                    srcRect = animationComponent->getCurrentFrameRect();
                    destW = static_cast<float>(srcRect.w);
                    destH = static_cast<float>(srcRect.h);
                }
            }

            SDL_FlipMode flip = SDL_FLIP_NONE;
            if (renderComponent->getFlipX()) {
                flip = static_cast<SDL_FlipMode>(flip | SDL_FLIP_HORIZONTAL);
            }
            if (renderComponent->getFlipY()) {
                flip = static_cast<SDL_FlipMode>(flip | SDL_FLIP_VERTICAL);
            }

            renderManager_.renderTexture(renderComponent->getTexture(), destX, destY, &srcRect, destW, destH, flip);
        }
    }
}
