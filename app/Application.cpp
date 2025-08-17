#include "Application.h"
#include <chrono>
#include <iostream>


// Managers are included in header file.
/* --- Include All Systems to use --- */
#include "engine/system/AnimationSystem.h"
#include "engine/system/RenderSystem.h"
#include "engine/system/MovementSystem.h"
#include "engine/system/InputSystem.h"
#include "engine/system/SoundSystem.h"
#include "engine/system/CameraSystem.h" 
#include "engine/system/InputToAccelerationSystem.h"
#include "engine/system/PlayerAnimationControlSystem.h"

/* --- Include All Components to use --- */
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

/* --- Include All Scenes to use --- */
#include "scene/MainMenuScene.h"
#include "scene/TestScene.h"


Application::Application() {}
Application::~Application() {
    std::cerr << "App is successfully quited.\n";
}

int Application::init(){
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) { /* Initialize SDL Systems*/
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return -1;
    }

    auto& fileManager = FileManager::getInstance();
    fileManager.firstInit();
    
    windowWidth = std::stoi(fileManager.getSetting("windowWidth", "1920"));
    windowHeight = std::stoi(fileManager.getSetting("windowHeight", "1080"));

    window_ = SDL_CreateWindow("Application", windowWidth, windowHeight, 0);
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if(!window_ || !renderer_){
        SDL_Log("Error occured in SDL_CreateWindow or SDL_CreateRenderer : %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    /* Set additional settings */
    SDL_SetRenderVSync(renderer_, true); /* Enable VSync */


    /* --- Initialize all manager --- */
    entityManager_ = std::make_unique<EntityManager>();
    systemManager_ = std::make_unique<SystemManager>(*entityManager_);
    renderManager_ = std::make_unique<RenderManager>(renderer_, window_);
    eventManager_ = std::make_unique<EventManager>();
    inputManager_ = std::make_unique<InputManager>(*eventManager_);
    textureManager_ = std::make_unique<TextureManager>(renderer_);
    textManager_ = std::make_unique<TextManager>(renderer_);
    soundManager_ = std::make_unique<SoundManager>();
    animationManager_ = std::make_unique<AnimationManager>();

    sceneManager_ = std::make_unique<SceneManager>();

    /* --- Regist all systems --- */
    systemManager_->registerSystem<RenderSystem>(SystemPhase::RENDER, *renderManager_, *textManager_);
    systemManager_->registerSystem<InputSystem>(SystemPhase::PRE_UPDATE, *eventManager_, *entityManager_);
    systemManager_->registerSystem<PlayerAnimationControlSystem>(SystemPhase::LOGIC_UPDATE, *animationManager_, *textureManager_, *renderManager_);
    systemManager_->registerSystem<SoundSystem>(SystemPhase::LOGIC_UPDATE, *soundManager_);
    systemManager_->registerSystem<CameraSystem>(SystemPhase::POST_UPDATE, *renderManager_);
    systemManager_->registerSystem<AnimationSystem>(SystemPhase::POST_UPDATE);
    systemManager_->registerSystem<InputToAccelerationSystem>(SystemPhase::PRE_UPDATE, *eventManager_, *entityManager_);
    systemManager_->registerSystem<MovementSystem>(SystemPhase::PHYSICS_UPDATE);

    

    /* --- Regist all Conpontnt to use --- */
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


    /* --- Regist all scane to use*/
    sceneManager_->addScene("TestScene", std::make_unique<TestScene>(*eventManager_, *renderManager_, *textureManager_, *soundManager_, *animationManager_, *entityManager_));
    sceneManager_->addScene("MainMenuScene", std::make_unique<MainMenuScene>());


    sceneManager_->changeScene("TestScene");

    lastFrameTime_ = std::chrono::high_resolution_clock::now();
    return 0;
}

void Application::quit() {
    auto& fileManager = FileManager::getInstance();
    const std::filesystem::path configPath = "app/data/config.bin";
    
    // Get current window size and save it
    SDL_GetWindowSize(window_, &windowWidth, &windowHeight);
    fileManager.setSetting("WindowWidth", std::to_string(windowWidth));
    fileManager.setSetting("WindowHeight", std::to_string(windowHeight));
    fileManager.saveSettings(configPath);

    std::cout << "cleaning up and quitting... " << std::endl;

    /* All Manager need no destruction */

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    SDL_Quit();
}


/**
 * @brief 애플리케이션의 메인 루프.
 * 이벤트 처리, 업데이트, 렌더링을 반복함.
 */
void Application::run() {
    // std::cerr << "[DEBUG] Application::run() - Entered run loop function.\n";
    isRunning_ = true;
    while(isRunning_){
        // std::cerr << "[DEBUG] Application::run() - Top of the main loop.\n";
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime_).count();
        lastFrameTime_ = currentTime;

        /* Process all events */
        if(!inputManager_->processEvents()){
            // std::cerr << "[DEBUG] Application::run() - processEvents() returned false. Exiting loop.\n";
            isRunning_ = false;
            break;
        }
        inputManager_->updateKeyStates();
        
        
        renderManager_->clear();
        
        /*
        * SystemManager perform  in the sequence. {PRE_UPDATE, LOGIT_UPDATE, PHYSICS_UPDATE, POST_UPDATE, RENDER}
        */
        // std::cerr << "[DEBUG] Application::run() - Calling systemManager_->updateAll().\n";
        systemManager_->updateAll(deltaTime);
        // std::cerr << "[DEBUG] Application::run() - Finished systemManager_->updateAll().\n";

        // std::cerr << "[DEBUG] Application::run() - Calling sceneManager_->update()\n";
        sceneManager_->update(deltaTime);
        
        renderManager_->present();
    }
}