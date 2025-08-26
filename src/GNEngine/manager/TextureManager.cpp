#include "GNEngine/manager/TextureManager.h"

#include <iostream>
#include <filesystem>
#include <memory>
#include <SDL3_image/SDL_image.h>

#include "GNEngine/resource/embedded/image/ErrorImage.h"

TextureManager::TextureManager(SDL_Renderer* renderer)
    : renderer_(renderer) {
    if (!renderer) {
        SDL_Log("TextureManager::init - rawRenderer is null: %s", SDL_GetError());
    }

    // Load the embedded default texture
    if (loadTextureEmbedded("__DEFAULT__", GNEngine::resource::embedded::EmbeddedLogo_png, sizeof(GNEngine::resource::embedded::EmbeddedLogo_png))) {
        defaultTexture_ = std::move(embeddedTextureMap_.at("__DEFAULT__"));
        embeddedTextureMap_.erase("__DEFAULT__");
    } else {
        SDL_Log("TextureManager::init - FAILED TO LOAD EMBEDDED DEFAULT TEXTURE");
        defaultTexture_ = nullptr;
    }
}

TextureManager::~TextureManager() {
    textureMap_.clear();
    embeddedTextureMap_.clear();
    std::cerr << "TextureManager " << this << " is successfully destroyed" << std::endl;
}

bool TextureManager::loadTexture(const std::filesystem::path& filePath) {
    if (textureMap_.count(filePath)) {
        return true;
    }

    if (!std::filesystem::exists(filePath)) {
        SDL_Log("TextureManager::loadTexture - File does not exist: %s", filePath.string().c_str());
        return false;
    }

    SDL_Surface* tmpSurface = IMG_Load(filePath.string().c_str());
    if (tmpSurface == nullptr) {
        SDL_Log("TextureManager::loadTexture - Failed to load surface %s: %s", filePath.string().c_str(), SDL_GetError());
        return false;
    }

    SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(renderer_, tmpSurface);
    SDL_DestroySurface(tmpSurface);

    if (sdlTexture == nullptr) {
        SDL_Log("TextureManager::loadTexture - Failed to create texture from surface %s: %s", filePath.string().c_str(), SDL_GetError());
        return false;
    }

    if (SDL_SetTextureScaleMode(sdlTexture, SDL_SCALEMODE_NEAREST) != 0) {
        SDL_Log("TextureManager::loadTexture - Failed to set texture scale mode for %s: %s", filePath.string().c_str(), SDL_GetError());
        // Even if setting scale mode fails, we still want to use the texture if it was created successfully.
        // This log helps diagnose, but doesn't prevent texture usage.
    }

    textureMap_[filePath] = std::make_unique<Texture>(sdlTexture, sdlTexture->w, sdlTexture->h);
    return true;
}

bool TextureManager::loadTextureEmbedded(const std::string& name, const unsigned char* data, size_t size) {
    if (embeddedTextureMap_.count(name)) {
        return true;
    }

    SDL_IOStream* io = SDL_IOFromConstMem(data, size);
    if (io == nullptr) {
        SDL_Log("Failed to create IOStream from memory for %s: %s", name.c_str(), SDL_GetError());
        return false;
    }

    SDL_Texture* sdlTexture = IMG_LoadTexture_IO(renderer_, io, true);
    if (sdlTexture == nullptr) {
        SDL_Log("Failed to load embedded texture %s: %s", name.c_str(), SDL_GetError());
        return false;
    }

    if (SDL_SetTextureScaleMode(sdlTexture, SDL_SCALEMODE_NEAREST) != 0) {
        SDL_Log("TextureManager::loadTextureEmbedded - Failed to set texture scale mode for %s: %s", name.c_str(), SDL_GetError());
        // Even if setting scale mode fails, we still want to use the texture if it was created successfully.
        // This log helps diagnose, but doesn't prevent texture usage.
    }

    embeddedTextureMap_[name] = std::make_unique<Texture>(sdlTexture);
    return true;
}

Texture* TextureManager::getTexture(const std::filesystem::path& filePath) {
    if (!textureMap_.count(filePath)) {
        if (!loadTexture(filePath)) {
            SDL_Log("TextureManager::getTexture - Texture not found and could not be loaded: %s. Returning default texture.", filePath.string().c_str());
            return defaultTexture_.get();
        }
    }
    return textureMap_.at(filePath).get();
}

Texture* TextureManager::getTexture(const std::string& name) {
    auto it = embeddedTextureMap_.find(name);
    if (it != embeddedTextureMap_.end()) {
        return it->second.get();
    }

    SDL_Log("TextureManager::getTexture - Embedded texture not found: %s. Returning default texture.", name.c_str());
    return defaultTexture_.get();
}

