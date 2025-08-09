#include "Application.h"
#include <chrono>
#include <iostream>

// 필요한 시스템 헤더들을 포함
#include "engine/system/InputSystem.h"
#include "engine/system/InputToAccelerationSystem.h"
#include "engine/system/MovementSystem.h"
#include "engine/system/PlayerAnimationControlSystem.h"
#include "engine/system/AnimationSystem.h"
#include "engine/system/SoundSystem.h"
#include "engine/system/CameraSystem.h"
#include "engine/system/RenderSystem.h"
#include "engine/system/AccelerationResetSystem.h"

#include "scene/TestScene.h"
#include "engine/component/InputControlComponent.h"

Application::Application() {};

int Application::init(){
    
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) { /* Initialize SDL Systems*/
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return -1;
    }
    
    window_ = SDL_CreateWindow("Application", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if(!window_ || !renderer_){
        SDL_Log("Error occured in SDL_CreateWindow or SDL_CreateRenderer : %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    /* Set additional settings */
    SDL_SetRenderVSync(renderer_, true); /* Enable VSync */


    /* 매니저 초기화 */
    eventManager_ = std::make_unique<EventManager>();
    entityManager_ = std::make_unique<EntityManager>();
    inputManager_ = std::make_unique<InputManager>(*eventManager_);
    renderManager_ = std::make_unique<RenderManager>(renderer_, window_);
    textureManager_ = std::make_unique<TextureManager>(renderer_);
    textManager_ = std::make_unique<TextManager>(renderer_);
    soundManager_ = std::make_unique<SoundManager>();
    animationManager_ = std::make_unique<AnimationManager>();
    
    // SystemManager 초기화
    systemManager_ = std::make_unique<SystemManager>(*entityManager_);

    // 시스템 등록
    
    systemManager_->registerSystem<InputSystem>(SystemPhase::PRE_UPDATE, *eventManager_, *entityManager_);
    systemManager_->registerSystem<InputToAccelerationSystem>(SystemPhase::PRE_UPDATE, *eventManager_, *entityManager_); // 가속도 설정 시스템
    
    systemManager_->registerSystem<PlayerAnimationControlSystem>(SystemPhase::LOGIC_UPDATE, *animationManager_, *textureManager_, *renderManager_);
    systemManager_->registerSystem<SoundSystem>(SystemPhase::LOGIC_UPDATE, *soundManager_);

    systemManager_->registerSystem<MovementSystem>(SystemPhase::PHYSICS_UPDATE);

    systemManager_->registerSystem<AnimationSystem>(SystemPhase::POST_UPDATE);
    systemManager_->registerSystem<CameraSystem>(SystemPhase::POST_UPDATE, *renderManager_);

    systemManager_->registerSystem<RenderSystem>(SystemPhase::RENDER, *renderManager_);

    sceneManager_ = std::make_unique<SceneManager>( eventManager_.get(), renderManager_.get(), textureManager_.get(), soundManager_.get(), entityManager_.get() );

    // 모든 컴포넌트 타입 등록
    entityManager_->registerComponentType<RenderComponent>();
    entityManager_->registerComponentType<AnimationComponent>();
    entityManager_->registerComponentType<SoundComponent>();
    entityManager_->registerComponentType<TextComponent>();
    entityManager_->registerComponentType<TransformComponent>();
    entityManager_->registerComponentType<VelocityComponent>();
    entityManager_->registerComponentType<AccelerationComponent>();
    entityManager_->registerComponentType<PlayerAnimationControllerComponent>();
    entityManager_->registerComponentType<InputControlComponent>();
    entityManager_->registerComponentType<CameraComponent>();


    // TestScene 등록 및 전환
    sceneManager_->addScene("TestScene", std::make_unique<TestScene>(*eventManager_, *renderManager_, *textureManager_, *soundManager_, *animationManager_, *entityManager_));
    sceneManager_->addScene("MainMenuScene", std::make_unique<MainMenuScene>(*eventManager_, *renderManager_, *textureManager_, *soundManager_, *entityManager_));
    sceneManager_->changeScene("TestScene");

    lastFrameTime_ = std::chrono::high_resolution_clock::now();
    return 0;
}

void Application::quit() {
    std::cout << "cleaning up and quitting... " << std::endl;

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    /* Manager들은 모두 자동으로 소멸함. */

    SDL_Quit();
}

/**
 * @brief 애플리케이션의 메인 루프.
 * 이벤트 처리, 업데이트, 렌더링을 반복함.
 */
void Application::run() {
    // SDL_Log("Application::run() - Entered run loop function.");
    isRunning_ = true;
    while(isRunning_){
        // SDL_Log("Application::run() - Top of the main loop.");
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime_).count();
        lastFrameTime_ = currentTime;

        /* Process all events */
        if(!inputManager_->processEvents()){
            // SDL_Log("Application::run() - processEvents() returned false. Exiting loop.");
            isRunning_ = false;
            break;
        }
        inputManager_->updateKeyStates();

        /* Update all systems */
        sceneManager_->update(deltaTime);

        renderManager_->clear();
        // SDL_Log("Application::run() - Calling systemManager_->updateAll().");
        systemManager_->updateAll(deltaTime);
        // SDL_Log("Application::run() - Finished systemManager_->updateAll().");
        renderManager_->present();
    }
}
