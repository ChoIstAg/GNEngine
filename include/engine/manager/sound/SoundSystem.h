#pragma once

#include "engine/ecs/EntityManager.h"
#include "engine/component/SoundComponent.h"
#include "engine/component/TransformComponent.h"
#include "engine/manager/sound/SoundManager.h"
#include <vector>

/*
 * @class SoundSystem
 * @brief SoundComponent와 TransformComponent를 가진 엔티티의 사운드 위치를 업데이트하고 사운드를 재생하는 시스템임.
*/
class SoundSystem {
public:
    SoundSystem(SoundManager& soundManager);

    /*
     * @brief 모든 사운드 가능한 엔티티의 사운드 위치를 업데이트하고 재생함.
     * @param entityManager - 엔티티와 컴포넌트를 관리하는 EntityManager.
     */
    void update(EntityManager& entityManager);

private:
    SoundManager& soundManager_;
};
