#include "Application.h"
#include <chrono>
#include <iostream>

#include "scene/TestScene.h" 

Application::Application() {};

int Application::init(){
    
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0) { /* Initialize SDL Systems*/
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return -1;
    }
    
    window_ = SDL_CreateWindow("Text main callback", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    
    
    if(!window_ || !renderer_){
        SDL_Log("Error occured in SDL_CreateWindow : %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    /* Set additional settings */
    SDL_SetRenderVSync(renderer_, true); /* Enable VSync */
    
    /* 매니저 초기화*/
    eventManager_ = std::make_unique<EventManager>();
    entityManager_ = std::make_unique<EntityManager>();

    // 모든 컴포넌트 타입 등록
    entityManager_->registerComponentType<TransformComponent>();
    entityManager_->registerComponentType<VelocityComponent>();
    entityManager_->registerComponentType<AccelerationComponent>();
    entityManager_->registerComponentType<RenderComponent>();
    entityManager_->registerComponentType<AnimationComponent>();
    entityManager_->registerComponentType<SoundComponent>();
    entityManager_->registerComponentType<TextComponent>();
    entityManager_->registerComponentType<PlayerAnimationControllerComponent>();
    entityManager_->registerComponentType<PlayerMovementComponent>();

    renderManager_ = std::make_unique<RenderManager>(renderer_, window_);
    renderSystem_ = std::make_unique<RenderSystem>(*renderManager_);
    
    inputManager_ = std::make_unique<InputManager>(*eventManager_);
    textureManager_ = std::make_unique<TextureManager>(renderer_);
    textManager_ = std::make_unique<TextManager>(renderer_);
    
    soundManager_ = std::make_unique<SoundManager>();
    soundSystem_ = std::make_unique<SoundSystem>(*soundManager_);
    animationManager_ = std::make_unique<AnimationManager>();
    animationSystem_ = std::make_unique<AnimationSystem>();
    movementSystem_ = std::make_unique<MovementSystem>();
    playerAnimationControlSystem_ = std::make_unique<PlayerAnimationControlSystem>(*animationManager_, *textureManager_, *renderManager_);
    inputSystem_ = std::make_unique<InputSystem>(*eventManager_, *entityManager_);
    
    sceneManager_ = std::make_unique<SceneManager>(
        eventManager_.get(),
        renderManager_.get(),
        textureManager_.get(),
        soundManager_.get(),
        entityManager_.get()
    );

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
    isRunning_ = true;
    while(isRunning_){
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime_).count();
        lastFrameTime_ = currentTime;

        /* Process Input */
        if(!inputManager_->processEvents()){
            isRunning_ = false;
            break;
        }

        /* Update */
        inputManager_->updateKeyStates();
        sceneManager_->update(deltaTime);
        inputSystem_->update(*entityManager_, deltaTime);
        animationSystem_->update(*entityManager_, deltaTime);
        movementSystem_->update(*entityManager_, deltaTime);
        playerAnimationControlSystem_->update(*entityManager_, deltaTime);
        soundSystem_->update(*entityManager_);

        /* Render */
        renderManager_->clear();
        
        renderSystem_->update(*entityManager_);
        renderManager_->present();
    }
}