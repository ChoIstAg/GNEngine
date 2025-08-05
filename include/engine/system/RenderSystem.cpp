#include "RenderSystem.h"
#include <iostream>

RenderSystem::RenderSystem(RenderManager& renderManager)
    : renderManager_(renderManager) {}

void RenderSystem::update(EntityManager& entityManager, CameraComponent* activeCamera) {
    float cameraX = 0.0f;
    float cameraY = 0.0f;
    float cameraZoom = 1.0f;

    if (activeCamera) {
        cameraX = activeCamera->x;
        cameraY = activeCamera->y;
        cameraZoom = activeCamera->zoom;
    }

    for (EntityId entity : entityManager.getEntitiesWith<TransformComponent, RenderComponent>()) {
        TransformComponent* transform = entityManager.getComponent<TransformComponent>(entity);
        RenderComponent* render = entityManager.getComponent<RenderComponent>(entity);
        AnimationComponent* animation = entityManager.getComponent<AnimationComponent>(entity);

        Texture* texture = render->getTexture();
        if (!texture) {
            std::cerr << "Error: RenderComponent has null texture. Skipping rendering for entity " << entity << std::endl;
            continue;
        }

        // 카메라 변환 적용
        // 엔티티의 월드 좌표를 카메라 기준으로 변환
        // 줌의 중심을 화면 중앙으로 맞추기 위해 조정
        float screenCenterX = renderManager_.getWindowWidth() / 2.0f;
        float screenCenterY = renderManager_.getWindowHeight() / 2.0f;

        float renderX = (transform->positionX_ - cameraX) * cameraZoom + screenCenterX - (screenCenterX * cameraZoom);
        float renderY = (transform->positionY_ - cameraY) * cameraZoom + screenCenterY - (screenCenterY * cameraZoom);

        // 렌더링할 너비와 높이도 줌에 따라 조정
        float renderW = texture->width_ * cameraZoom;
        float renderH = texture->height_ * cameraZoom;

        SDL_Rect currentFrameSrcRect; // Declare outside if/else

        if (render->hasAnimation() && animation) {
            currentFrameSrcRect = animation->getCurrentFrameRect();
            // 렌더링할 너비와 높이를 srcRect의 크기에 따라 조정
            renderW = static_cast<float>(currentFrameSrcRect.w) * cameraZoom;
            renderH = static_cast<float>(currentFrameSrcRect.h) * cameraZoom;
        } else {
            // If no animation, use the entire texture as the source rectangle
            currentFrameSrcRect = {0, 0, texture->width_, texture->height_};
        }

        SDL_FlipMode flip = SDL_FLIP_NONE;
        if (render->getFlipX()) {
            flip = static_cast<SDL_FlipMode>(flip | SDL_FLIP_HORIZONTAL);
        }
        if (render->getFlipY()) {
            flip = static_cast<SDL_FlipMode>(flip | SDL_FLIP_VERTICAL);
        }

        renderManager_.renderTexture(texture, renderX, renderY, &currentFrameSrcRect, renderW, renderH, flip);
    }
}
