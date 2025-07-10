#pragma once
#include <SDL3/SDL.h>

#include "TextureManager.h"
#include "InputManager.h"
//#include "Managers.h"

class Application{
private:
    SDL_Renderer* renderer_;
    SDL_Window* window_;
    
    int windowWidth = 1280;
    int windowHeight = 720;

public:
    Application();
    int init();
    void quit();

    SDL_Renderer* getRenderer() const { return renderer_; }
    SDL_Window* getWindow() const { return window_; }


    /* Managers */
    TextureManager textureManager;
    InputManager inputManager;
};