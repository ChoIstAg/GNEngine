#include "engine/system/RenderSystem.h"
#include <iostream>
#include <SDL3/SDL_render.h>

RenderSystem::RenderSystem(RenderManager& renderManager, TextManager& textManager)
    : renderManager_(renderManager), textManager_(textManager) {}

void RenderSystem::update(EntityManager& entityManager, float deltaTime) {
    // 1. 필요한 모든 컴포넌트 배열들을 가져옴
    auto renderArray = entityManager.getComponentArray<RenderComponent>();
    auto transformArray = entityManager.getComponentArray<TransformComponent>();
    auto animArray = entityManager.getComponentArray<AnimationComponent>();
    auto textArray = entityManager.getComponentArray<TextComponent>();

    // 렌더링 가능한 엔티티 처리 (RenderComponent)
    if (renderArray && transformArray) {
        auto& textures = renderArray->textures;
        auto& hasAnims = renderArray->hasAnimations;
        auto& srcX = renderArray->srcRectX;
        auto& srcY = renderArray->srcRectY;
        auto& srcW = renderArray->srcRectW;
        auto& srcH = renderArray->srcRectH;
        auto& flipX = renderArray->flipX;
        auto& flipY = renderArray->flipY;

        auto& posX = transformArray->positionX;
        auto& posY = transformArray->positionY;
        
        auto entities = entityManager.getEntitiesWith<RenderComponent, TransformComponent>();
        const auto& entityToIndexMap = renderArray->getEntityToIndexMap();

        for (const auto& entity : entities) {
            const size_t i = entityToIndexMap.at(entity);

            if (textures[i]) {
                SDL_Rect srcRect = { srcX[i], srcY[i], srcW[i], srcH[i] };
                float destW = static_cast<float>(srcRect.w);
                float destH = static_cast<float>(srcRect.h);

                if (hasAnims[i] && animArray && animArray->hasComponent(entity)) {
                    const size_t animIndex = animArray->getEntityToIndexMap().at(entity);
                    const auto& animation = animArray->animations[animIndex];
                    if (animation) {
                        srcRect = animation->getFrame(animArray->currentFrames[animIndex]);
                        destW = static_cast<float>(srcRect.w);
                        destH = static_cast<float>(srcRect.h);
                    }
                }

                SDL_FlipMode flip = SDL_FLIP_NONE;
                if (flipX[i]) {
                    flip = static_cast<SDL_FlipMode>(flip | SDL_FLIP_HORIZONTAL);
                }
                if (flipY[i]) {
                    flip = static_cast<SDL_FlipMode>(flip | SDL_FLIP_VERTICAL);
                }

                renderManager_.renderTexture(textures[i], posX[i], posY[i], &srcRect, destW, destH, flip);
            }
        }
    }

    // 텍스트 컴포넌트 처리 (TextComponent)
    if (textArray && transformArray) {
        auto& texts = textArray->texts;
        auto& fontPaths = textArray->fontPaths;
        auto& fontSizes = textArray->fontSizes;
        auto& colorsR = textArray->colorsR;
        auto& colorsG = textArray->colorsG;
        auto& colorsB = textArray->colorsB;
        auto& colorsA = textArray->colorsA;
        auto& areDirty = textArray->areDirty;
        auto& textTextures = textArray->textures;
        auto& textureWidths = textArray->textureWidths;
        auto& textureHeights = textArray->textureHeights;

        auto& posX = transformArray->positionX;
        auto& posY = transformArray->positionY;

        auto textEntities = entityManager.getEntitiesWith<TextComponent, TransformComponent>();
        const auto& textEntityToIndexMap = textArray->getEntityToIndexMap();

        for (const auto& entity : textEntities) {
            const size_t i = textEntityToIndexMap.at(entity);

            // 텍스트가 변경되었으면 텍스처 재생성
            if (areDirty[i]) {
                if (textTextures[i] != nullptr) {
                    SDL_DestroyTexture(textTextures[i]);
                    textTextures[i] = nullptr;
                }
                SDL_Color color = {colorsR[i], colorsG[i], colorsB[i], colorsA[i]};
                
                auto tempText = textManager_.createText(fontPaths[i], texts[i], color, false, false, 0, 0);
                if (tempText) {
                    textTextures[i] = tempText->getSDLTexture();
                    SDL_GetTextureSize(textTextures[i], &textureWidths[i], &textureHeights[i]);
                } else {
                    textTextures[i] = nullptr; // 텍스처 생성 실패
                }
                areDirty[i] = false;
            }

            // 텍스처가 있으면 렌더링
            if (textTextures[i] != nullptr) {
                renderManager_.renderTexture(textTextures[i], posX[i], posY[i], nullptr, static_cast<float>(textureWidths[i]), static_cast<float>(textureHeights[i]), SDL_FLIP_NONE);
            }
        }
    }
}