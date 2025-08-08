#pragma once
#include "../../GNEngine_API.h"

#include "engine/manager/EntityManager.h"
#include "engine/core/SystemInterface.h"
#include "engine/component/AccelerationComponent.h"
#include "engine/component/InputControlComponent.h"

/**
 * @class AccelerationResetSystem
 * @brief 매 프레임 시작 시 모든 AccelerationComponent를 0으로 리셋하는 시스템.
 * 
 * 이 시스템은 PRE_UPDATE 단계에서 가장 먼저 실행되어,
 * 이전 프레임의 가속도 값을 초기화하고 새로운 입력에 의한 가속도 설정을 준비함.
 */
class GNEngine_API AccelerationResetSystem : public SystemInterface {
public:
    AccelerationResetSystem() = default;

    void update(EntityManager& entityManager, float deltaTime) override;
};