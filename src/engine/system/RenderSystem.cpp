#include "engine/system/RenderSystem.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL3/SDL_render.h>

#include "engine/core/Entity.h"
#include "engine/core/RenderLayer.h"
#include "engine/component/FadeComponent.h"

RenderSystem::RenderSystem(RenderManager& renderManager, TextManager& textManager)
    : renderManager_(renderManager), textManager_(textManager) {}

void RenderSystem::update(EntityManager& entityManager, float deltaTime) {
    // 이 함수 내에서만 사용할 로컬 구조체와 정렬용 람다
    struct Renderable {
        EntityID entity;
        RenderLayer layer;
    };

    auto compareRenderables = [](const Renderable& a, const Renderable& b) {
        return static_cast<int>(a.layer) < static_cast<int>(b.layer);
    };

    std::vector<Renderable> renderables;

    // 1. RenderComponent가 있는 엔티티 수집
    auto renderArray = entityManager.getComponentArray<RenderComponent>();
    if (renderArray) {
        for (const auto& entity : entityManager.getEntitiesWith<RenderComponent, TransformComponent>()) {
            const auto& renderComponent = renderArray->getComponent(entity);
            renderables.push_back({entity, renderComponent.getLayer()});
        }
    }

    // 2. TextComponent가 있는 엔티티 수집
    auto textArray = entityManager.getComponentArray<TextComponent>();
    if (textArray) {
        for (const auto& entity : entityManager.getEntitiesWith<TextComponent, TransformComponent>()) {
            const auto& textComponent = textArray->getComponent(entity);
            renderables.push_back({entity, textComponent.layer});
        }
    }

    // 3. 레이어 기준으로 모든 렌더링 대상 정렬
    std::sort(renderables.begin(), renderables.end(), compareRenderables);

    // 4. 정렬된 순서대로 렌더링
    auto transformArray = entityManager.getComponentArray<TransformComponent>();
    auto animArray = entityManager.getComponentArray<AnimationComponent>();
    auto fadeArray = entityManager.getComponentArray<FadeComponent>(); // FadeComponent 배열 가져오기

    for (const auto& renderable : renderables) {
        EntityID entity = renderable.entity;

        // TransformComponent는 필수
        if (!transformArray->hasComponent(entity)) continue;
        const auto& transform = transformArray->getComponent(entity);

        // RenderComponent 처리
        if (renderArray && renderArray->hasComponent(entity)) {
            const auto& render = renderArray->getComponent(entity);
            if (render.getTexture()) {
                SDL_Rect srcRect = render.getSrcRect();
                float destW = static_cast<float>(srcRect.w);
                float destH = static_cast<float>(srcRect.h);

                if (render.hasAnimation() && animArray && animArray->hasComponent(entity)) {
                    const auto& anim = animArray->getComponent(entity);
                    if (anim.animation_) { // animation_이 유효한지 확인
                        srcRect = anim.animation_->getFrame(anim.currentFrame_); // currentFrame_ 사용
                        destW = static_cast<float>(srcRect.w);
                        destH = static_cast<float>(srcRect.h);
                    }
                }

                SDL_FlipMode flip = SDL_FLIP_NONE;
                if (render.getFlipX()) flip = static_cast<SDL_FlipMode>(flip | SDL_FLIP_HORIZONTAL);
                if (render.getFlipY()) flip = static_cast<SDL_FlipMode>(flip | SDL_FLIP_VERTICAL);

                renderManager_.renderTexture(render.getTexture(), transform.positionX_, transform.positionY_, &srcRect, destW, destH, flip);
            } else {
                // 텍스처가 없는 RenderComponent는 페이드 효과로 간주
                if (fadeArray && fadeArray->hasComponent(entity)) {
                    const auto& fade = fadeArray->getComponent(entity);
                    SDL_Renderer* renderer = renderManager_.getRenderer();
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(renderer, fade.color.r, fade.color.g, fade.color.b, static_cast<Uint8>(fade.currentAlpha));

                    int w, h;
                    SDL_GetRenderOutputSize(renderer, &w, &h);
                    SDL_FRect fadeRect = { 0, 0, static_cast<float>(w), static_cast<float>(h) };

                    SDL_RenderFillRect(renderer, &fadeRect);
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                }
            }
        }

        // TextComponent 처리
        if (textArray && textArray->hasComponent(entity)) {
            // TextComponent의 데이터를 직접 접근하여 수정
            const size_t textIndex = textArray->getEntityToIndexMap().at(entity);
            
            if (textArray->areDirty[textIndex]) {
                if (textArray->textures[textIndex] != nullptr) {
                    SDL_DestroyTexture(textArray->textures[textIndex]);
                    textArray->textures[textIndex] = nullptr;
                }
                SDL_Color color = {textArray->colorsR[textIndex], textArray->colorsG[textIndex], textArray->colorsB[textIndex], textArray->colorsA[textIndex]};
                
                auto tempText = textManager_.createText(textArray->fontPaths[textIndex], textArray->texts[textIndex], color, false, false, 0, 0);
                if (tempText) {
                    textArray->textures[textIndex] = tempText->getSDLTexture();
                    float texW, texH;
                    SDL_GetTextureSize(textArray->textures[textIndex], &texW, &texH);
                    textArray->textureWidths[textIndex] = texW;
                    textArray->textureHeights[textIndex] = texH;
                } else {
                    textArray->textures[textIndex] = nullptr;
                }
                textArray->areDirty[textIndex] = false;
            }

            if (textArray->textures[textIndex] != nullptr) {
                renderManager_.renderTexture(textArray->textures[textIndex], transform.positionX_, transform.positionY_, nullptr, static_cast<float>(textArray->textureWidths[textIndex]), static_cast<float>(textArray->textureHeights[textIndex]), SDL_FLIP_NONE);
            }
        }
    }
}