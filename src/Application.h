#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include <memory>
#include <chrono>

#include "engine/manager/event/InputManager.h"
#include "engine/manager/event/EventManager.h"
#include "engine/manager/render/RenderManager.h"
#include "engine/manager/resource/TextureManager.h"
#include "engine/manager/resource/TextManager.h"
#include "engine/manager/resource/AnimationManager.h"
#include "engine/component/TextComponent.h"
#include "engine/manager/resource/AnimationSystem.h"
#include "engine/manager/sound/SoundManager.h"
#include "engine/manager/scene/SceneManager.h"
#include "engine/manager/render/RenderSystem.h"
#include "engine/manager/event/MovementSystem.h"
#include "engine/manager/event/InputSystem.h"
#include "engine/manager/resource/PlayerAnimationControlSystem.h"
#include "engine/manager/sound/SoundSystem.h"
#include "engine/ecs/EntityManager.h"
#include "scene/MainMenuScene.h"

class Application {
private:
    SDL_Renderer* renderer_;
    SDL_Window* window_;
    
    int windowWidth = 1280;
    int windowHeight = 720;
    bool isRunning_ = false;

    std::chrono::high_resolution_clock::time_point lastFrameTime_;

public:
    Application();
    int init();
    void quit();

    void run();

    SDL_Renderer* getRenderer() const { return renderer_; }
    SDL_Window* getWindow() const { return window_; }

    /* Managers */
    std::unique_ptr<EventManager> eventManager_;
    std::unique_ptr<InputManager> inputManager_;
    std::unique_ptr<TextureManager> textureManager_;
    std::unique_ptr<RenderManager> renderManager_;
    std::unique_ptr<TextManager> textManager_;
    std::unique_ptr<SoundManager> soundManager_;
    std::unique_ptr<SceneManager> sceneManager_;
    std::unique_ptr<AnimationManager> animationManager_;
    std::unique_ptr<RenderSystem> renderSystem_;
    std::unique_ptr<AnimationSystem> animationSystem_;
    std::unique_ptr<MovementSystem> movementSystem_;
    std::unique_ptr<PlayerAnimationControlSystem> playerAnimationControlSystem_;
    std::unique_ptr<SoundSystem> soundSystem_;
    std::unique_ptr<EntityManager> entityManager_;
    std::unique_ptr<InputSystem> inputSystem_;
};