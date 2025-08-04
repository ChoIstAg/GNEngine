#pragma once

#include "Component.h"
#include "engine/resource/animation/Animation.h"
#include <memory>
#include <filesystem>

/*
 * @class PlayerAnimationControllerComponent
 * @brief 플레이어 캐릭터의 애니메이션 로딩, 관리 및 전환을 위한 데이터를 담는 컴포넌트임.
 *        로직은 PlayerAnimationControlSystem에서 처리함.
*/
struct PlayerAnimationControllerComponent : public Component {
public:
    std::shared_ptr<Animation> walkAnimationData_;
    std::shared_ptr<Animation> jumpAnimationData_;

    // 생성자 (데이터 초기화용)
    PlayerAnimationControllerComponent(
        std::shared_ptr<Animation> walkAnim,
        std::shared_ptr<Animation> jumpAnim
    ) : walkAnimationData_(walkAnim), jumpAnimationData_(jumpAnim) {}
};
