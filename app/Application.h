#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_hints.h>

#include <memory>
#include <chrono>

#include "engine/system/AnimationSystem.h"
#include "engine/system/RenderSystem.h"
#include "engine/system/MovementSystem.h"
#include "engine/system/InputSystem.h"
#include "engine/system/SoundSystem.h"
#include "engine/system/CameraSystem.h" 
#include "engine/system/InputToAccelerationSystem.h"
#include "engine/system/PlayerAnimationControlSystem.h"

#include "engine/manager/EventManager.h"
#include "engine/manager/InputManager.h"
#include "engine/manager/EntityManager.h"
#include "engine/manager/RenderManager.h"
#include "engine/manager/TextureManager.h"
#include "engine/manager/TextManager.h"
#include "engine/manager/AnimationManager.h"
#include "engine/manager/SoundManager.h"
#include "engine/manager/SceneManager.h"
#include "engine/manager/SystemManager.h"

#include "engine/component/SoundComponent.h"
#include "engine/component/CameraComponent.h"
#include "engine/component/RenderComponent.h"
#include "engine/component/AccelerationComponent.h"
#include "engine/component/VelocityComponent.h"
#include "engine/component/InputControlComponent.h"
#include "engine/component/TransformComponent.h"
#include "engine/component/AnimationComponent.h"
#include "engine/component/TextComponent.h"
#include "engine/component/PlayerAnimationControllerComponent.h"
#include "engine/component/PlayerMovementComponent.h"


/* --- Include All scenes to use --- */
#include "scene/MainMenuScene.h"
#include "scene/TestScene.h"

class Application {
private:
    SDL_Renderer* renderer_;
    SDL_Window* window_;
    
    int windowWidth = WINDOW_WIDTH;
    int windowHeight = WINDOW_HEIGHT;

    bool isRunning_ = false;

    std::chrono::high_resolution_clock::time_point lastFrameTime_;

public:
    Application();
    int init();

    void run();
    void quit();

    SDL_Renderer* getRenderer() const { return renderer_; }
    SDL_Window* getWindow() const { return window_; }

    /* Managers */
    std::unique_ptr<EventManager> eventManager_;
    std::unique_ptr<EntityManager> entityManager_;
    std::unique_ptr<InputManager> inputManager_;
    std::unique_ptr<RenderManager> renderManager_;
    std::unique_ptr<SceneManager> sceneManager_;
    std::unique_ptr<TextureManager> textureManager_;
    std::unique_ptr<TextManager> textManager_;
    std::unique_ptr<AnimationManager> animationManager_;
    std::unique_ptr<SoundManager> soundManager_;
    std::unique_ptr<SystemManager> systemManager_;
};