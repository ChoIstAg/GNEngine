#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include <memory> // For std::unique_ptr

#include "engine/manager/event/InputManager.h"
#include "engine/manager/event/EventManager.h"
#include "engine/manager/render/RenderManager.h"
#include "engine/manager/resource/TextureManager.h"
#include "engine/manager/resource/TextManager.h"

#include "engine/_legacy/TestObject.h"
#include "engine/_legacy/TextObject.h"

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
    std::unique_ptr<TextManager> textManager_;
    std::unique_ptr<TextObject> testText_;
    std::unique_ptr<TestObject> testObject_; 
};