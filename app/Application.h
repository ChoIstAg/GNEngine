#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_hints.h>

#include <memory>
#include <chrono>

/* --- Include all Manager to use --- */
#include "engine/manager/FileManager.h"
#include "engine/manager/EntityManager.h"
#include "engine/manager/SceneManager.h"
#include "engine/manager/SystemManager.h"
#include "engine/manager/RenderManager.h"
#include "engine/manager/EventManager.h"
#include "engine/manager/InputManager.h"
#include "engine/manager/SoundManager.h"
#include "engine/manager/TextureManager.h"
#include "engine/manager/TextManager.h"
#include "engine/manager/AnimationManager.h"
#include "engine/manager/FadeManager.h"

class Application {
private:
    SDL_Renderer* renderer_;
    SDL_Window* window_;
    
    int windowWidth;
    int windowHeight;

    bool isRunning_ = false;

    std::chrono::high_resolution_clock::time_point lastFrameTime_;

public:
    Application();
    ~Application();
    int init();

    void run();
    void quit();

    SDL_Renderer* getRenderer() const { return renderer_; }
    SDL_Window* getWindow() const { return window_; }

    /* 
     * Managers
     * The destruction order is the reverse of the declaration order.
     * Declare in the order of creation/dependency.
    */
    std::unique_ptr<EventManager> eventManager_;
    std::unique_ptr<InputManager> inputManager_;
    std::unique_ptr<SoundManager> soundManager_;
    std::unique_ptr<TextureManager> textureManager_;
    std::unique_ptr<TextManager> textManager_;
    std::unique_ptr<AnimationManager> animationManager_;
    std::unique_ptr<EntityManager> entityManager_;
    std::unique_ptr<SystemManager> systemManager_;
    std::unique_ptr<SceneManager> sceneManager_;
    std::unique_ptr<FadeManager> fadeManager_;
    std::unique_ptr<RenderManager> renderManager_; // RenderManager often depends on windowing, should be last.
};
