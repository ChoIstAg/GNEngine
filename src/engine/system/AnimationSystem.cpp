#include "engine/system/AnimationSystem.h"
#include <iostream>
#include <string>
void AnimationSystem::update(EntityManager& entityManager, float deltaTime) {
    //std::cout << "AnimationSystem::update entered. DeltaTime: " << deltaTime << std::endl;
    for (EntityId entity : entityManager.getEntitiesWith<AnimationComponent>()) {
        AnimationComponent* animationComponent = entityManager.getComponent<AnimationComponent>(entity);

        // debugging
        std::string errorMessage;
        if(!animationComponent->isPlaying_) { errorMessage = "AnimationComponent is not playing"; }
        if(!animationComponent->animation_) { errorMessage = "AnimationComponent has no animation"; }
        if(animationComponent->animation_ && animationComponent->animation_->getFrameCount() == 0) {
            errorMessage = "AnimationComponent's animation has no frames";
        }
        if (!animationComponent->isPlaying_ || !animationComponent->animation_ || animationComponent->animation_->getFrameCount() == 0) {
            std::cout << "  Skipping animation update for entity " << entity << ": isPlaying_=" << animationComponent->isPlaying_
                      << ", animation_=" << (animationComponent->animation_ ? "valid" : "nullptr")
                      << ", FrameCount=" << (animationComponent->animation_ ? std::to_string(animationComponent->animation_->getFrameCount()) : "N/A") 
                      << "error message: " << errorMessage << "\n";
            continue;
        }

        animationComponent->frameTimer_ += deltaTime;

        float currentFrameDuration = static_cast<float>(animationComponent->animation_->getFrameDuration(animationComponent->currentFrame_)) / 1000.0f;

        // std::cout << "  Entity: " << entity
        //      << ", FrameTimer: " << animationComponent->frameTimer_
        //      << ", CurrentFrameDuration: " << currentFrameDuration << std::endl;


        if (animationComponent->frameTimer_ >= currentFrameDuration) {
            animationComponent->frameTimer_ -= currentFrameDuration;
            animationComponent->currentFrame_++;

            if (animationComponent->currentFrame_ >= animationComponent->animation_->getFrameCount()) {
                if (animationComponent->animation_->isLooping()) {
                    animationComponent->currentFrame_ = 0;
                } else {
                    animationComponent->currentFrame_ = animationComponent->animation_->getFrameCount() - 1;
                    animationComponent->isPlaying_ = false;
                    animationComponent->isFinished_ = true;
                }
            }
        }
        const SDL_Rect& currentRect = animationComponent->animation_->getFrame(animationComponent->currentFrame_);
        // std::cout << "  Entity: " << entity
        //           << ", Current Frame: " << animationComponent->currentFrame_
        //           << ", SrcRect: {x:" << currentRect.x
        //           << ", y:" << currentRect.y
        //           << ", w:" << currentRect.w
        //           << ", h:" << currentRect.h << "}" << std::endl;
    }
}
