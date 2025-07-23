#pragma once
#include <SDL3/SDL.h>

#include <memory> // For std::unique_ptr

#include "engine/resource/TextureManager.h"
#include "engine/event/InputManager.h"
#include "engine/event/EventManager.h"
#include "engine/event/RenderManager.h"

#include "engine/object/BlankObject.h"

class Application {
private:
    SDL_Renderer* renderer_;
    SDL_Window* window_;
    
    int windowWidth = 1280;
    int windowHeight = 720;
    bool isRunning_ = false;

public:
    Application();
    int init();
    void quit();
    void run();

    SDL_Renderer* getRenderer() const { return renderer_; }
    SDL_Window* getWindow() const { return window_; }

    /* Managers */
    EventManager eventManager_;
    InputManager inputManager_;
    TextureManager textureManager_;
    RenderManager renderManager_;
    std::unique_ptr<BlankObject> blankObject_; // BlankObject를 unique_ptr로 변경
};