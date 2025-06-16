#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <fstream>
#include <sstream>

#include <print>

class TextObject{
    TTF_Font* font;
    SDL_Texture* textTexture;
    SDL_Surface* textSurface;
    SDL_FRect dstRect;

    ~TextObject();
};

class TextManager{
private:
    std::vector<TextObject> textObject;
    SDL_Renderer* renderer;
    bool preLoad;

public:
    TextManager(SDL_Renderer* renderer): renderer(renderer){};
    ~TextManager();
    void load_textfile(const std::string& filepath);
};