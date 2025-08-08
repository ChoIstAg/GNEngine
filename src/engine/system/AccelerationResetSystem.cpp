#include "engine/system/AccelerationResetSystem.h"

void AccelerationResetSystem::update(EntityManager& entityManager, float deltaTime) {
    // InputControlComponent를 가진 엔티티의 가속도만 리셋 (입력으로 제어되는 엔티티만 해당)
    for (auto entity : entityManager.getEntitiesWith<InputControlComponent, AccelerationComponent>()) {
        auto* acceleration = entityManager.getComponent<AccelerationComponent>(entity);
        acceleration->ax = 0.0f;
        acceleration->ay = 0.0f;
    }
}
