#include "engine/system/FadeSystem.h"

#include <algorithm>
#include <iostream>

#include "engine/component/FadeComponent.h"
#include "engine/manager/RenderManager.h"
#include "engine/manager/EntityManager.h"

/*
 * @brief FadeSystem의 생성자.
 * @param renderManager 렌더링에 사용할 RenderManager의 포인터.
 */
FadeSystem::FadeSystem(RenderManager& renderManager) 
    : renderManager_(renderManager) {}

/*
 * @brief 매 프레임 호출되어 페이드 상태를 업데이트하고 화면에 그립니다.
 * @param entityManager 엔티티와 컴포넌트를 관리하는 EntityManager.
 * @param deltaTime 이전 프레임과의 시간 간격.
 */
void FadeSystem::update(EntityManager& entityManager, float deltaTime) {
    auto fadeArray = entityManager.getComponentArray<FadeComponent>();
    if (!fadeArray) {
        // std::cerr << "[DEBUG] FadeSystem - ComponentArray<FadeComponent> is nullptr \n";
        return;
    }

    auto entities = entityManager.getEntitiesWith<FadeComponent>();

    for (auto entity : entities) {
        auto& fade = fadeArray->getComponent(entity);

        if (fade.state == FadeState::NONE) {
            continue;
        }

        fade.timer += deltaTime;
        float progress = (fade.duration > 0) ? (fade.timer / fade.duration) : 1.0f;
        progress = std::min(progress, 1.0f);

        float alpha = 0.0f;
        if (fade.state == FadeState::FADE_IN) {
            alpha = 255.0f * (1.0f - progress);
        } else { // FADE_OUT
            alpha = 255.0f * progress;
        }

        if (fade.timer >= fade.duration) {
            alpha = (fade.state == FadeState::FADE_IN) ? 0.0f : 255.0f;
            fade.state = FadeState::NONE;
            fade.timer = 0.0f;

            if (fade.onComplete) {
                fade.onComplete();
                fade.onComplete = nullptr;
            }
        }

        /* 화면 전체를 덮는 사각형을 그림 */
        SDL_Renderer* renderer = renderManager_.getRenderer();
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, fade.color.r, fade.color.g, fade.color.b, static_cast<Uint8>(alpha));
        
        int w, h;
        SDL_GetRenderOutputSize(renderer, &w, &h);
        SDL_FRect fadeRect = { 0, 0, static_cast<float>(w), static_cast<float>(h) };

        SDL_RenderFillRect(renderer, &fadeRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
}