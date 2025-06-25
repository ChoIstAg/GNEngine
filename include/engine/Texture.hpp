#pragma once
#include <SDL3/SDL.h>
#include <string>

/* A Texture. */
using Texture = class Texture{
public:
    SDL_Texture* texture;
    SDL_FRect dstRect;
    float scaleX, scaleY;

    ~Texture();
    bool loadTexture( std::string filePath_, float x_, float y_, float scaleX_, float scaleY_ );
};