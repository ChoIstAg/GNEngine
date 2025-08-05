#include "InputToAccelerationSystem.h"

void InputToAccelerationSystem::update(EntityManager& entityManager, float deltaTime) {
    for (EntityId entity : entityManager.getEntitiesWith<InputControlComponent, AccelerationComponent>()) {
        InputControlComponent* inputControl = entityManager.getComponent<InputControlComponent>(entity);
        AccelerationComponent* acceleration = entityManager.getComponent<AccelerationComponent>(entity);

        // InputControlComponent의 desiredX/Y를 기반으로 가속도 설정
        acceleration->ax = inputControl->desiredX * ACCELERATION_RATE;
        acceleration->ay = inputControl->desiredY * ACCELERATION_RATE;
    }
}