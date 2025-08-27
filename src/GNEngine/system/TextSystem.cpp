#include "GNEngine/system/TextSystem.h"
#include "GNEngine/core/Entity.h"

#include <SDL3_ttf/SDL_ttf.h>

TextSystem::TextSystem(EntityManager& entityManager, TextManager& textManager, SDL_Renderer* renderer)
    : entityManager_(entityManager), textManager_(textManager), renderer_(renderer) {
}

TextSystem::~TextSystem() {
}

void TextSystem::update(float deltaTime) {
    for (auto& entity : entityManager_.getEntitiesWith<TextComponent, RenderComponent>()) {
        auto textComponentOpt = entityManager_.getComponent<TextComponent>(entity);
        auto renderComponentOpt = entityManager_.getComponent<RenderComponent>(entity);

        if (textComponentOpt && renderComponentOpt && textComponentOpt->isDirty) {
            auto& textComponent = *textComponentOpt;
            auto& renderComponent = *renderComponentOpt;

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

            renderComponent.setSDLTexture(newTexture, textureWidth, textureHeight);
            textComponent.isDirty = false;
        }
    }
}
