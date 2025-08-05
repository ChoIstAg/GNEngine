#include "CameraSystem.h"
#include <iostream>

CameraSystem::CameraSystem(RenderManager& renderManager)
    : renderManager_(renderManager) {}

void CameraSystem::update(EntityManager& entityManager, float deltaTime) {
    int windowWidth = renderManager_.getWindowWidth();
    int windowHeight = renderManager_.getWindowHeight();

    for (EntityId entity : entityManager.getEntitiesWith<CameraComponent>()) {
        CameraComponent* camera = entityManager.getComponent<CameraComponent>(entity);

        if (camera->targetEntityId != 0) {
            // 타겟 엔티티가 존재하고 TransformComponent를 가지고 있다면 카메라 위치 업데이트
            if (entityManager.hasComponent<TransformComponent>(camera->targetEntityId)) {
                TransformComponent* targetTransform = entityManager.getComponent<TransformComponent>(camera->targetEntityId);
                
                // 카메라를 타겟 엔티티의 위치로 이동 (간단한 따라가기 로직)
                // 플레이어를 화면 중앙에 위치시키기 위해 화면 크기의 절반만큼 오프셋 적용
                camera->x = targetTransform->positionX_ - (windowWidth / 2.0f);
                camera->y = targetTransform->positionY_ - (windowHeight / 2.0f);

                // TODO: 부드러운 카메라 이동, 경계 처리, 줌 레벨 조정 등 추가 로직 구현
            }
        }
        // TODO: 줌 레벨 조정 로직 등 추가
    }
}
