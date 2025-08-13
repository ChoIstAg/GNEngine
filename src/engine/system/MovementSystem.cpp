#include "engine/system/MovementSystem.h"
#include <cmath>
#include "config.h" // Include config.h for DECELERATION_RATE, ACCELERATION_RATE, MOVE_SPEED

#include <iostream>

void MovementSystem::update(EntityManager& entityManager, float deltaTime) {
    // 1. 필요한 컴포넌트 배열에 대한 포인터를 가져옴
    auto transformArray = entityManager.getComponentArray<TransformComponent>();
    auto velocityArray = entityManager.getComponentArray<VelocityComponent>();
    auto accelerationArray = entityManager.getComponentArray<AccelerationComponent>();

    // 컴포넌트 배열이 없으면 시스템을 실행할 수 없음
    if (!transformArray || !velocityArray || !accelerationArray) {
        std::cerr << "[Error] MovementSystem - ComponentArray is nullptr.\n";
        return;
    }

    // 2. SoA 데이터에 직접 접근하기 위해 각 컴포넌트의 데이터 벡터를 가져옴
    auto& posX = transformArray->positionX;
    auto& posY = transformArray->positionY;
    auto& velX = velocityArray->vx;
    auto& velY = velocityArray->vy;
    auto& accX = accelerationArray->ax;
    auto& accY = accelerationArray->ay;

    // 3. 처리할 엔티티 목록을 가져옴
    auto entities = entityManager.getEntitiesWith<TransformComponent, VelocityComponent, AccelerationComponent>();
    const auto& entityToIndexMap = transformArray->getEntityToIndexMap();

    // 4. 엔티티 루프를 돌며 데이터 처리 (캐시 친화적 방식)
    for (const auto& entity : entities) {
        const size_t i = entityToIndexMap.at(entity); // 엔티티 ID로부터 데이터 인덱스를 찾음

        // 가속도를 이용한 속도 업데이트
        velX[i] += accX[i] * deltaTime;
        velY[i] += accY[i] * deltaTime;

        // 가속도가 없을 때 감속 적용
        if (accX[i] == 0.0f) {
            if (velX[i] > 0) {
                velX[i] = std::max(0.0f, velX[i] - DECELERATION_RATE * deltaTime);
            } else if (velX[i] < 0) {
                velX[i] = std::min(0.0f, velX[i] + DECELERATION_RATE * deltaTime);
            }
        }
        if (accY[i] == 0.0f) {
            if (velY[i] > 0) {
                velY[i] = std::max(0.0f, velY[i] - DECELERATION_RATE * deltaTime);
            } else if (velY[i] < 0) {
                velY[i] = std::min(0.0f, velY[i] + DECELERATION_RATE * deltaTime);
            }
        }

        // 속도 제한
        if (std::abs(velX[i]) > MAX_SPEED) {
            velX[i] = std::copysign(MAX_SPEED, velX[i]);
        }
        if (std::abs(velY[i]) > MAX_SPEED) {
            velY[i] = std::copysign(MAX_SPEED, velY[i]);
        }

        // 속도를 이용한 위치 업데이트
        posX[i] += velX[i] * deltaTime;
        posY[i] += velY[i] * deltaTime;

        // 매 프레임 끝에 가속도를 0으로 리셋
        accX[i] = 0.0f;
        accY[i] = 0.0f;
    }
}