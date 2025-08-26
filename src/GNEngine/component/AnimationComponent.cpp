#include "GNEngine/component/AnimationComponent.h"
#include <iostream>

/*
 * @brief AnimationComponent 생성자.
 * @param animation - 이 컴포넌트가 재생할 Animation 데이터.
 * @param playOnAwake - 컴포넌트 생성 시 바로 애니메이션을 재생할지 여부 (기본값: true).
 */
AnimationComponent::AnimationComponent(std::shared_ptr<Animation> animation, bool playOnAwake)
    : animation_(std::move(animation)),
      currentFrame_(0),
      frameTimer_(0.0f),
      isPlaying_(playOnAwake),
      isFinished_(false) {}

/*
 * @brief 애니메이션 재생을 시작함.
 */
void AnimationComponent::play() {
    isPlaying_ = true;
    // If the animation was finished, reset to the first frame
    if (isFinished_) {
        currentFrame_ = 0;
        frameTimer_ = 0.0f;
    }
    isFinished_ = false;
}

/*
 * @brief 애니메이션 재생을 일시 정지함.
 */
void AnimationComponent::pause() {
    isPlaying_ = false;
}

/*
 * @brief 애니메이션 재생을 중지하고 첫 프레임으로 되돌림.
 */
void AnimationComponent::stop() {
    isPlaying_ = false;
    isFinished_ = false;
    currentFrame_ = 0;
    frameTimer_ = 0.0f;
}

/*
 * @brief 현재 재생 중인 프레임의 스프라이트 시트 내 사각형 영역을 반환함.
 * RenderComponent에서 이 정보를 사용하여 올바른 프레임을 렌더링함.
 * @return 현재 프레임의 SDL_Rect.
 */
const SDL_Rect& AnimationComponent::getCurrentFrameRect() const {
    if (animation_) {
        return animation_->getFrame(currentFrame_);
    }
    static const SDL_Rect emptyRect = {0, 0, 0, 0};
    return emptyRect;
}



