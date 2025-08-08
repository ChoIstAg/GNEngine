#include "engine/manager/SystemManager.h"

SystemManager::SystemManager(EntityManager& entityManager)
    : entityManager_(entityManager) {}

void SystemManager::registerSystem(std::unique_ptr<SystemInterface> system, SystemPhase phase) {
    systems_[phase].push_back(std::move(system));
}

void SystemManager::updateAll(float deltaTime) {
    // SystemPhase에 정의된 순서대로 시스템들을 업데이트
    
    // 1. PRE_UPDATE
    if (systems_.count(SystemPhase::PRE_UPDATE)) {
        for (auto& system : systems_.at(SystemPhase::PRE_UPDATE)) {
            system->update(entityManager_, deltaTime);
        }
    }

    // 2. LOGIC_UPDATE
    if (systems_.count(SystemPhase::LOGIC_UPDATE)) {
        for (auto& system : systems_.at(SystemPhase::LOGIC_UPDATE)) {
            system->update(entityManager_, deltaTime);
        }
    }

    // 3. PHYSICS_UPDATE
    if (systems_.count(SystemPhase::PHYSICS_UPDATE)) {
        for (auto& system : systems_.at(SystemPhase::PHYSICS_UPDATE)) {
            system->update(entityManager_, deltaTime);
        }
    }

    // 4. POST_UPDATE
    if (systems_.count(SystemPhase::POST_UPDATE)) {
        for (auto& system : systems_.at(SystemPhase::POST_UPDATE)) {
            system->update(entityManager_, deltaTime);
        }
    }

    // 5. RENDER
    if (systems_.count(SystemPhase::RENDER)) {
        for (auto& system : systems_.at(SystemPhase::RENDER)) {
            system->update(entityManager_, deltaTime);
        }
    }
}