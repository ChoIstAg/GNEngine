#include "engine/system/AnimationSystem.h"
#include <iostream>

void AnimationSystem::update(EntityManager& entityManager, float deltaTime) {
    auto animArray = entityManager.getComponentArray<AnimationComponent>();
    if (!animArray) {
        std::cerr << "AnimationSystem.cpp에서 참조 실패 \n";
        return;
    }

    auto& animations = animArray->animations;    
    auto& currentFrames = animArray->currentFrames;
    auto& frameTimers = animArray->frameTimers;
    auto& arePlaying = animArray->arePlaying;
    auto& areFinished = animArray->areFinished;

    auto entities = entityManager.getEntitiesWith<AnimationComponent>();
    const auto& entityToIndexMap = animArray->getEntityToIndexMap();

    for (const auto& entity : entities) {
        const size_t i = entityToIndexMap.at(entity);

        if (!arePlaying[i] || !animations[i] || animations[i]->getFrameCount() == 0) {
            continue;
        }

        frameTimers[i] += deltaTime;

        float currentFrameDuration = static_cast<float>(animations[i]->getFrameDuration(currentFrames[i])) / 1000.0f;

        if (frameTimers[i] >= currentFrameDuration) {
            frameTimers[i] -= currentFrameDuration;
            currentFrames[i]++;

            if (currentFrames[i] >= animations[i]->getFrameCount()) {
                if (animations[i]->isLooping()) {
                    currentFrames[i] = 0;
                } else {
                    currentFrames[i] = animations[i]->getFrameCount() - 1;
                    arePlaying[i] = false;
                    areFinished[i] = true;
                }
            }
        }
    }
}