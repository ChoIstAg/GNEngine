#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "Texture.h"
#include "config.h"


class TextureManager {
private:    
    SDL_Renderer* rawRenderer_; /* Raw renderer actually used in main */
    bool preLoad_ = true; /* if preLoad is true, Texture is loaded when this class is allocated*/
    int count_ = 0; /* The number of texture */
    std::unordered_map<std::string, Texture*> textureMap_; /* Normal texture name : texture value*/
    std::unordered_map<std::string, Texture*> backgroundMap_;

public:
    TextureManager();
    ~TextureManager();

    bool init(SDL_Renderer* renderer);
    bool loadTexture(std::string filePath, float scaleX, float scaleY); 
    
    Texture* getTexture(std::string filePath);
    
    float getPosition(std::string filePath, char position); /* ex: ('x') -> return x. (x,y,w,h)*/
    bool setPosition(std::string filePath, float x, float y);
};