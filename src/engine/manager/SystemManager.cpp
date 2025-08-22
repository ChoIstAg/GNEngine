#include "GNEngine/manager/SystemManager.h"
#include <iostream>

SystemManager::SystemManager(EntityManager& entityManager)
    : entityManager_(entityManager) {}

SystemManager::~SystemManager() {
    std::cerr << "SystemManager " << this << " is successfully destroyed. \n" ;
}

void SystemManager::updateAll(float deltaTime) {
    // SystemPhase에 정의된 순서대로 시스템들을 업데이트
    
    // 1. PRE_UPDATE
    if (systems_.count(SystemPhase::PRE_UPDATE)) {
        for (auto& updateFunc : systems_.at(SystemPhase::PRE_UPDATE)) {
            updateFunc(deltaTime);
        }
    }
    // std::cerr << "SystemManager - Finished PRE_UPDATE \n";

    // 2. LOGIC_UPDATE
    if (systems_.count(SystemPhase::LOGIC_UPDATE)) {
        for (auto& updateFunc : systems_.at(SystemPhase::LOGIC_UPDATE)) {
            updateFunc(deltaTime);
        }
    }
    // std::cerr << "SystemManager - Finished LOGIC_UPDATE \n";

    // 3. PHYSICS_UPDATE
    if (systems_.count(SystemPhase::PHYSICS_UPDATE)) {
        for (auto& updateFunc : systems_.at(SystemPhase::PHYSICS_UPDATE)) {
            updateFunc(deltaTime);
        }
    }
    // std::cerr << "SystemManager - Finished PHYSICS_UPDATE \n";

    // 4. POST_UPDATE
    if (systems_.count(SystemPhase::POST_UPDATE)) {
        for (auto& updateFunc : systems_.at(SystemPhase::POST_UPDATE)) {
            updateFunc(deltaTime);
        }
    }
    // std::cerr << "SystemManager - Finished POST_UPDATE \n";

    // 5. RENDER
    if (systems_.count(SystemPhase::RENDER)) {
        for (auto& updateFunc : systems_.at(SystemPhase::RENDER)) {
            updateFunc(deltaTime);
        }
    }
    // std::cerr << "SystemManager - Finished RENDER \n";
}
