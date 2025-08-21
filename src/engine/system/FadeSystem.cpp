#include "engine/system/FadeSystem.h"

#include <algorithm>
#include <iostream>

#include "engine/manager/RenderManager.h"
#include "engine/manager/EntityManager.h"

#include "engine/component/FadeComponent.h"

/*
 * @brief FadeSystem의 생성자.
 * @param renderManager 렌더링에 사용할 RenderManager의 포인터.
 */
FadeSystem::FadeSystem(RenderManager& renderManager) 
    : renderManager_(renderManager) {}

//TODO [5] - fade 적용 로직을 변경하여 남은 시간과 지속시간을 유동적으로 변경할 수 있게 만들기. (LogoScene에서 키 입력시 지속시간 감소, 빨리 스킵.)
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

        if (fade.state == FadeState::FADE_IN) {
            fade.currentAlpha = 255.0f * (1.0f - progress);
        } else { // FADE_OUT
            fade.currentAlpha = 255.0f * progress;
        }

        if (fade.timer >= fade.duration) {
            fade.currentAlpha = (fade.state == FadeState::FADE_IN) ? 0.0f : 255.0f;
            fade.state = FadeState::NONE;
            fade.timer = 0.0f;

            if (fade.onComplete) {
                fade.onComplete();
                fade.onComplete = nullptr;
            }

            // 페이드가 완료되면 엔티티를 파괴
            entityManager.destroyEntity(entity);
        }
    }
}
