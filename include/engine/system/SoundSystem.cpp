#include "SoundSystem.h"
#include <iostream>

SoundSystem::SoundSystem(SoundManager& soundManager)
    : soundManager_(soundManager) {}

void SoundSystem::update(EntityManager& entityManager) {
    for (EntityId entity : entityManager.getEntitiesWith<SoundComponent, TransformComponent>()) {
        SoundComponent* sound = entityManager.getComponent<SoundComponent>(entity);
        TransformComponent* transform = entityManager.getComponent<TransformComponent>(entity);

        // 사운드 위치 업데이트
        sound->setPosition(transform->positionX_, transform->positionY_, 0.0f);
    }
}
