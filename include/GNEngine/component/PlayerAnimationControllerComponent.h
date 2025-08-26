#pragma once
#include "../GNEngine_API.h"

#include <memory>
#include <filesystem>

#include "GNEngine/core/Component.h"
#include "GNEngine/core/Animation.h"

/*
 * @class PlayerAnimationControllerComponent
 * @brief 플레이어 캐릭터의 애니메이션 로딩, 관리 및 전환을 위한 데이터를 담는 컴포넌트임.
 *        로직은 PlayerAnimationControlSystem에서 처리함.
 * @param walkAnimation 걷기 애니메이션 데이터.
 * @param jumpAnimation 점프 애니메이션 데이터.
 * @param idleAnimation 대기 애니메이션 데이터.
*/ 
class GNEngine_API PlayerAnimationControllerComponent : public Component {
public:
    std::shared_ptr<Animation> walkAnimationData_;
    std::shared_ptr<Animation> jumpAnimationData_;
    std::shared_ptr<Animation> idleAnimationData_;

    // 생성자 (데이터 초기화용)
    PlayerAnimationControllerComponent(
        std::shared_ptr<Animation> walkAnimation,
        std::shared_ptr<Animation> jumpAnimation,
        std::shared_ptr<Animation> idleAnimation
    ) : walkAnimationData_(walkAnimation), jumpAnimationData_(jumpAnimation), idleAnimationData_(idleAnimation) {}
};




