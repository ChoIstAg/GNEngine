#include "engine/manager/FadeManager.h"

#include "engine/manager/EntityManager.h"
#include "engine/component/FadeComponent.h"

FadeManager::FadeManager(EntityManager& entityManager) 
    : entityManager_(entityManager) {}

/*
 * @brief 페이드 인 효과 시작.
 * @param duration 페이드 인에 걸리는 시간 (초).
 * @param color 페이드 색상.
 * @param onComplete 페이드 완료 시 호출될 콜백 함수.
 */
void FadeManager::startFadeIn(float duration, SDL_Color color, std::function<void()> onComplete) {
    auto fadeEntity = entityManager_.createEntity();
    auto& fade = entityManager_.addComponent<FadeComponent>(fadeEntity);
    fade.state = FadeState::FADE_IN;
    fade.duration = duration;
    fade.color = color;
    fade.onComplete = onComplete;
}

/*
 * @brief 페이드 아웃 효과 시작
 * @param duration 페이드 아웃에 걸리는 시간 (초).
 * @param color 페이드 색상.
 * @param onComplete 페이드 완료 시 호출될 콜백 함수.
 */
void FadeManager::startFadeOut(float duration, SDL_Color color, std::function<void()> onComplete) {
    auto fadeEntity = entityManager_.createEntity();
    auto& fade = entityManager_.addComponent<FadeComponent>(fadeEntity);
    fade.state = FadeState::FADE_OUT;
    fade.duration = duration;
    fade.color = color;
    fade.onComplete = onComplete;
}
