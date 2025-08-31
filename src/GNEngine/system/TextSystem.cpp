#include "GNEngine/system/TextSystem.h"
#include "GNEngine/core/Entity.h"

#include <SDL3_ttf/SDL_ttf.h>

TextSystem::TextSystem(EntityManager& entityManager, TextManager& textManager, SDL_Renderer* renderer)
    : entityManager_(entityManager), textManager_(textManager), renderer_(renderer) {
}

TextSystem::~TextSystem() {
}

void TextSystem::update(EntityManager& entityManager, float deltaTime) {
    auto renderComponentArray = entityManager.getComponentArray<RenderComponent>();
    auto textComponentArray = entityManager.getComponentArray<TextComponent>();
    if (!renderComponentArray || !textComponentArray) return;

    for (auto& entity : entityManager_.getEntitiesWith<TextComponent, RenderComponent>()) {
        auto textComponentOpt = entityManager_.getComponent<TextComponent>(entity);

        if (textComponentOpt && textComponentOpt->isDirty) {
            auto& textComponent = *textComponentOpt;

            TTF_Font* font = textManager_.getFont(textComponent.fontPath, textComponent.fontSize);
            if (!font) {
                SDL_Log("TextSystem::update - Font not found for path: %s, size: %d", textComponent.fontPath.string().c_str(), textComponent.fontSize);
                continue;
            }

            SDL_Surface* surface = TTF_RenderText_Blended(font, textComponent.text.c_str(), textComponent.text.length(), textComponent.color);

            if (!surface) {
                SDL_Log("TextSystem::update - Failed to create surface from text: %s", SDL_GetError());
                continue;
            }

            SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer_, surface);
            if (!newTexture) {
                SDL_DestroySurface(surface);
                SDL_Log("TextSystem::update - Failed to create texture from surface: %s", SDL_GetError());
                continue;
            }

            int textureWidth = surface->w;
            int textureHeight = surface->h;

            SDL_DestroySurface(surface);

            renderComponentArray->updateTexture(entity, newTexture, textureWidth, textureHeight);
            textComponentArray->setDirty(entity, false);
            
            auto transformComp = entityManager.getComponent<TransformComponent>(entity);
            // SDL_Log("--- TEXT SYSTEM DEBUG --- Text: '%s' | Position: (%.2f, %.2f) | Texture Size: (%d, %d)", textComponent.text.c_str(), transformComp->positionX_, transformComp->positionY_, textureWidth, textureHeight);
        } else if (textComponentOpt && !textComponentOpt->isDirty) {
            // SDL_Log("TextSystem::update - TextComponent is not dirty, skipping texture regeneration.");
        }
    }
}