#include "engine/system/CameraSystem.h"
#include <iostream>

CameraSystem::CameraSystem(RenderManager& renderManager)
    : renderManager_(renderManager) {}

void CameraSystem::update(EntityManager& entityManager, float deltaTime) {
    SDL_Log("CameraSystem: update called.");
    int windowWidth = renderManager_.getWindowWidth();
    int windowHeight = renderManager_.getWindowHeight();

    for (EntityId entity : entityManager.getEntitiesWith<CameraComponent>()) {
        CameraComponent* camera = entityManager.getComponent<CameraComponent>(entity);

        if (camera->targetEntityId != 0) {
            SDL_Log("CameraSystem: Target entity ID is %u.", camera->targetEntityId);
            // 타겟 엔티티가 존재하고 TransformComponent를 가지고 있다면 카메라 위치 업데이트
            if (entityManager.hasComponent<TransformComponent>(camera->targetEntityId)) {
                TransformComponent* targetTransform = entityManager.getComponent<TransformComponent>(camera->targetEntityId);
                
                // 카메라를 타겟 엔티티의 위치로 이동 (간단한 따라가기 로직)
                camera->x = targetTransform->positionX_;
                camera->y = targetTransform->positionY_;

                // RenderManager에 카메라 위치 업데이트
                renderManager_.setCameraPosition(camera->x, camera->y);
                renderManager_.setZoomLevel(camera->zoom);
                SDL_Log("CameraSystem: Camera position set to (%.2f, %.2f) and zoom set to %.2f for target entity %u at (%.2f, %.2f).", camera->x, camera->y, camera->zoom, camera->targetEntityId, targetTransform->positionX_, targetTransform->positionY_);

                // TODO: 부드러운 카메라 이동, 경계 처리, 줌 레벨 조정 등 추가 로직 구현
            } else {
                SDL_Log("CameraSystem: Target entity %u does not have TransformComponent.", camera->targetEntityId);
            }
        } else {
            SDL_Log("CameraSystem: No target entity set for camera %u.", entity);
        }
        // TODO: 줌 레벨 조정 로직 등 추가
    }
}
