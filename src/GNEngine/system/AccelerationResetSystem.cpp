#include "GNEngine/system/AccelerationResetSystem.h"

void AccelerationResetSystem::update(EntityManager& entityManager, float deltaTime) {
    auto accelArray = entityManager.getComponentArray<AccelerationComponent>();
    if (!accelArray) {
        std::cerr << "AccelerationResetSystem.cpp에서 accelArray 참조 실패 \n";
        return;
    }

    auto& ax = accelArray->ax;
    auto& ay = accelArray->ay;

    for (auto entity : entityManager.getEntitiesWith<InputControlComponent, AccelerationComponent>()) {
        const size_t i = accelArray->getEntityToIndexMap().at(entity);
        ax[i] = 0.0f;
        ay[i] = 0.0f;
    }
}


