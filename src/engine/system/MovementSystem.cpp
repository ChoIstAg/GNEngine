#include "engine/system/MovementSystem.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <cmath>
#include "config.h" // Include config.h for DECELERATION_RATE, ACCELERATION_RATE, MOVE_SPEED
#include "engine/component/InputControlComponent.h" // InputControlComponent.h 추가

void MovementSystem::update(EntityManager& entityManager, float deltaTime) {
    // SDL_Log("MovementSystem: update called. DeltaTime: %.4f", deltaTime);
    for (EntityId entity : entityManager.getEntitiesWith<TransformComponent, VelocityComponent, AccelerationComponent>()) {
        TransformComponent* transform = entityManager.getComponent<TransformComponent>(entity);
        VelocityComponent* velocity = entityManager.getComponent<VelocityComponent>(entity);
        AccelerationComponent* acceleration = entityManager.getComponent<AccelerationComponent>(entity);

        SDL_Log("MovementSystem: BEFORE ACCEL RESET - Entity %u - Acc(%.2f, %.2f)", entity, acceleration->ax, acceleration->ay);

        

        SDL_Log("MovementSystem: AFTER ACCEL RESET - Entity %u - Acc(%.2f, %.2f), Vel(%.2f, %.2f), Pos(%.2f, %.2f)",
                entity, acceleration->ax, acceleration->ay, velocity->vx, velocity->vy, transform->positionX_, transform->positionY_);

        // 가속도를 이용한 속도 업데이트
        velocity->vx += acceleration->ax * deltaTime;
        velocity->vy += acceleration->ay * deltaTime;

        // 가속도가 없을 때 감속 적용
        if (acceleration->ax == 0.0f) {
            if (velocity->vx > 0) {
                velocity->vx = std::max(0.0f, velocity->vx - DECELERATION_RATE * deltaTime);
            } else if (velocity->vx < 0) {
                velocity->vx = std::min(0.0f, velocity->vx + DECELERATION_RATE * deltaTime);
            }
        }
        if (acceleration->ay == 0.0f) {
            if (velocity->vy > 0) {
                velocity->vy = std::max(0.0f, velocity->vy - DECELERATION_RATE * deltaTime);
            } else if (velocity->vy < 0) {
                velocity->vy = std::min(0.0f, velocity->vy + DECELERATION_RATE * deltaTime);
            }
        }

        // 속도 제한
        if (std::abs(velocity->vx) > MAX_SPEED) {
            velocity->vx = std::copysign(MAX_SPEED, velocity->vx);
        }
        if (std::abs(velocity->vy) > MAX_SPEED) {
            velocity->vy = std::copysign(MAX_SPEED, velocity->vy);
        }

        // 속도를 이용한 위치 업데이트
        transform->positionX_ += velocity->vx * deltaTime;
        transform->positionY_ += velocity->vy * deltaTime;

        // 매 프레임 끝에 가속도를 0으로 리셋
        acceleration->ax = 0.0f;
        acceleration->ay = 0.0f;
    }
}