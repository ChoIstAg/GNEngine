#pragma once
#include <SDL3/SDL.h>
#include <string>

/* A Texture. */
struct Texture{
public:
    SDL_Texture* texture;
    SDL_FRect dstRect;
    float scaleX, scaleY;
};