#include "GNEngine/system/MovementSystem.h"
#include "GNEngineRootPath.h"

#include <cmath>
#include <iostream>

void MovementSystem::update(EntityManager& entityManager, float deltaTime) {
    // 1. 필요한 컴포넌트 배열에 대한 포인터를 가져옴
    auto transformArray = entityManager.getComponentArray<TransformComponent>();
    auto velocityArray = entityManager.getComponentArray<VelocityComponent>();
    auto accelerationArray = entityManager.getComponentArray<AccelerationComponent>();

    // 컴포넌트 배열이 없으면 시스템을 실행할 수 없음
    if (!transformArray || !velocityArray || !accelerationArray) {
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

    // 4. 엔티티 루프를 돌며 데이터 처리
    for (const auto& entity : entities) {
        const size_t transformIndex = transformArray->getEntityToIndexMap().at(entity);
        const size_t velocityIndex = velocityArray->getEntityToIndexMap().at(entity);
        const size_t accelerationIndex = accelerationArray->getEntityToIndexMap().at(entity);

        // 가속도를 이용한 속도 업데이트
        velX[velocityIndex] += accX[accelerationIndex] * deltaTime;
        velY[velocityIndex] += accY[accelerationIndex] * deltaTime;

        // 가속도가 없을 때 감속 적용
        if (accX[accelerationIndex] == 0.0f) {
            if (velX[velocityIndex] > 0) {
                velX[velocityIndex] = std::max(0.0f, velX[velocityIndex] - DECELERATION_RATE * deltaTime);
            } else if (velX[velocityIndex] < 0) {
                velX[velocityIndex] = std::min(0.0f, velX[velocityIndex] + DECELERATION_RATE * deltaTime);
            }
        }
        if (accY[accelerationIndex] == 0.0f) {
            if (velY[velocityIndex] > 0) {
                velY[velocityIndex] = std::max(0.0f, velY[velocityIndex] - DECELERATION_RATE * deltaTime);
            } else if (velY[velocityIndex] < 0) {
                velY[velocityIndex] = std::min(0.0f, velY[velocityIndex] + DECELERATION_RATE * deltaTime);
            }
        }

        // 속도 제한
        if (std::abs(velX[velocityIndex]) > MAX_SPEED) {
            velX[velocityIndex] = std::copysign(MAX_SPEED, velX[velocityIndex]);
        }
        if (std::abs(velY[velocityIndex]) > MAX_SPEED) {
            velY[velocityIndex] = std::copysign(MAX_SPEED, velY[velocityIndex]);
        }

        // 속도를 이용한 위치 업데이트
        posX[transformIndex] += velX[velocityIndex] * deltaTime;
        posY[transformIndex] += velY[velocityIndex] * deltaTime;

        // 매 프레임 끝에 가속도를 0으로 리셋
        accX[accelerationIndex] = 0.0f;
        accY[accelerationIndex] = 0.0f;
    }
}



