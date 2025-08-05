#include "MovementSystem.h"
#include <iostream>
#include <cmath>
#include "config.h" // Include config.h for DECELERATION_RATE, ACCELERATION_RATE, MOVE_SPEED
#include "engine/component/InputControlComponent.h" // InputControlComponent.h 추가

void MovementSystem::update(EntityManager& entityManager, float deltaTime) {
    for (EntityId entity : entityManager.getEntitiesWith<TransformComponent, VelocityComponent, AccelerationComponent>()) {
        TransformComponent* transform = entityManager.getComponent<TransformComponent>(entity);
        VelocityComponent* velocity = entityManager.getComponent<VelocityComponent>(entity);
        AccelerationComponent* acceleration = entityManager.getComponent<AccelerationComponent>(entity);

        // 가속도 적용
        velocity->vx += acceleration->ax * deltaTime;
        velocity->vy += acceleration->ay * deltaTime;

        // 감속도 적용 (마찰 효과)
        velocity->vx *= (1.0f - DECELERATION_RATE * deltaTime);
        velocity->vy *= (1.0f - DECELERATION_RATE * deltaTime);

        // 속도 제한
        float currentSpeed = std::sqrt(velocity->vx * velocity->vx + velocity->vy * velocity->vy);
        if (currentSpeed > MOVE_SPEED) {
            velocity->vx = (velocity->vx / currentSpeed) * MOVE_SPEED;
            velocity->vy = (velocity->vy / currentSpeed) * MOVE_SPEED;
        }

        // 속도가 매우 작아지면 0으로 설정하여 부동 소수점 오차 누적 방지
        const float MIN_VELOCITY = 0.01f;
        if (std::abs(velocity->vx) < MIN_VELOCITY) velocity->vx = 0.0f;
        if (std::abs(velocity->vy) < MIN_VELOCITY) velocity->vy = 0.0f;

        // 위치 업데이트
        transform->positionX_ += velocity->vx * deltaTime;
        transform->positionY_ += velocity->vy * deltaTime;
    }
}
