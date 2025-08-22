#pragma once
#include "../../GNEngine_API.h"

#include "GNEngine/manager/EntityManager.h"
#include "GNEngine/component/SoundComponent.h"
#include "GNEngine/component/TransformComponent.h"
#include "GNEngine/manager/SoundManager.h"

#include <vector>

/*
 * @class SoundSystem
 * @brief SoundComponent와 TransformComponent를 가진 엔티티의 사운드 위치를 업데이트하고 사운드를 재생하는 시스템임.
*/
class GNEngine_API SoundSystem {
public:
    SoundSystem(SoundManager& soundManager);

    /*
     * @brief 모든 사운드 가능한 엔티티의 사운드 위치를 업데이트하고 재생함.
     * @param entityManager - 엔티티와 컴포넌트를 관리하는 EntityManager.
     * @param deltaTime - 이 시스템에서는 사용되지 않음.
     */
    void update(EntityManager& entityManager, float deltaTime);

private:
    SoundManager& soundManager_;
};
