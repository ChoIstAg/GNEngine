#pragma once

#include "Component.h"
#include "VelocityComponent.h"
#include "AccelerationComponent.h"
#include "engine/ecs/Entity.h"

/*
 * @class PlayerMovementComponent
 * @brief 플레이어의 이동 관련 데이터를 저장하는 컴포넌트임.
*/
class PlayerMovementComponent : public Component {
public:
    PlayerMovementComponent() = default;
    ~PlayerMovementComponent() = default;

    // 이동 생성자 및 이동 대입 연산자
    PlayerMovementComponent(PlayerMovementComponent&& other) noexcept = default;
    PlayerMovementComponent& operator=(PlayerMovementComponent&& other) noexcept = default;

    // 복사 생성자 및 복사 대입 연산자 삭제 (참조 멤버 때문)
    PlayerMovementComponent(const PlayerMovementComponent&) = delete;
    PlayerMovementComponent& operator=(const PlayerMovementComponent&) = delete;
};
