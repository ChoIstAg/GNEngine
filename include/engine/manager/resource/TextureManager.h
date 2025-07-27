#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <unordered_map>
#include <memory> // For std::unique_ptr

#include "engine/resource/texture/Texture.h"
#include "config.h"


class TextureManager {
private:    
    SDL_Renderer* renderer_; /* Raw renderer actually used in main */
    std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap_; /* Normal texture name : texture value*/

public:
    TextureManager(SDL_Renderer* renderer);
    ~TextureManager();

    bool loadTexture(const std::string& filePath); 
    
    Texture* getTexture(const std::string& filePath) const;
};