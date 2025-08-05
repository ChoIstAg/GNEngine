#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <unordered_map>
#include <memory> // For std::unique_ptr
#include <filesystem> // Ensure this is included
#include <functional> // For std::hash<std::filesystem::path>

#include "engine/resource/Texture.h"
#include "config.h"


class TextureManager {
private:    
    SDL_Renderer* renderer_; /* Raw renderer actually used in main */
    std::unordered_map<std::filesystem::path, std::unique_ptr<Texture>> textureMap_; /* Normal texture name : texture value*/

public:
    TextureManager(SDL_Renderer* renderer);
    ~TextureManager();

    bool loadTexture(const std::filesystem::path& filePath); 
    
    Texture* getTexture(const std::filesystem::path& filePath) const;
};