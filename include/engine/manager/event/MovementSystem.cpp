#include "MovementSystem.h"
#include <iostream>
#include <cmath>

void MovementSystem::update(EntityManager& entityManager, float deltaTime) {
    for (EntityId entity : entityManager.getEntitiesWith<TransformComponent, VelocityComponent>()) {
        TransformComponent* transform = entityManager.getComponent<TransformComponent>(entity);
        VelocityComponent* velocity = entityManager.getComponent<VelocityComponent>(entity);
        AccelerationComponent* acceleration = entityManager.getComponent<AccelerationComponent>(entity);

        // 가속도 적용 (있다면)
        if (acceleration) {
            velocity->vx += acceleration->ax * deltaTime;
            velocity->vy += acceleration->ay * deltaTime;
        }

        // 위치 업데이트
        transform->positionX_ += velocity->vx * deltaTime;
        transform->positionY_ += velocity->vy * deltaTime;

        // 감속도 적용 (선택 사항: 여기서는 간단한 마찰 효과)
        // velocity->vx *= (1.0f - 0.1f * deltaTime); // 예시: 10% 감속
        // velocity->vy *= (1.0f - 0.1f * deltaTime); // 예시: 10% 감속

        // 속도가 매우 작아지면 0으로 설정하여 부동 소수점 오차 누적 방지
        const float MIN_VELOCITY = 0.01f;
        if (std::abs(velocity->vx) < MIN_VELOCITY) velocity->vx = 0.0f;
        if (std::abs(velocity->vy) < MIN_VELOCITY) velocity->vy = 0.0f;
    }
}
