#include "AnimationSystem.h"
#include <iostream>

void AnimationSystem::update(EntityManager& entityManager, float deltaTime) {
    for (EntityId entity : entityManager.getEntitiesWith<AnimationComponent>()) {
        AnimationComponent* animationComponent = entityManager.getComponent<AnimationComponent>(entity);
        
        // 애니메이션 컴포넌트의 update 로직을 여기에 구현
        if (!animationComponent->isPlaying_ || !animationComponent->animation_ || animationComponent->animation_->getFrameCount() == 0) {
            continue;
        }

        animationComponent->frameTimer_ += deltaTime;

        // 현재 프레임의 지속 시간 (밀리초)을 초 단위로 변환
        float currentFrameDuration = static_cast<float>(animationComponent->animation_->getFrameDuration(animationComponent->currentFrame_)) / 1000.0f;

        if (animationComponent->frameTimer_ >= currentFrameDuration) {
            animationComponent->frameTimer_ -= currentFrameDuration; // 다음 프레임으로 넘어갈 때 남은 시간 처리
            animationComponent->currentFrame_++;

            if (animationComponent->currentFrame_ >= animationComponent->animation_->getFrameCount()) {
                if (animationComponent->animation_->isLooping()) {
                    animationComponent->currentFrame_ = 0; // 반복 애니메이션이면 처음으로
                } else {
                    animationComponent->currentFrame_ = animationComponent->animation_->getFrameCount() - 1; // 마지막 프레임에 고정
                    animationComponent->isPlaying_ = false; // 재생 중지
                    animationComponent->isFinished_ = true; // 완료 상태로 설정
                }
            }
        }
    }
}
