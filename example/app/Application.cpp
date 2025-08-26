#include "Application.h"
#include "AppRootPath.h"

#include <chrono>
#include <iostream>
#include <algorithm>
#include <filesystem>

#include "GNEngine/GNManager.h"

/* --- Include All Systems to use --- */
#include "GNEngine/system/AnimationSystem.h"
#include "GNEngine/system/RenderSystem.h"
#include "GNEngine/system/MovementSystem.h"
#include "GNEngine/system/InputSystem.h"
#include "GNEngine/system/SoundSystem.h"
#include "GNEngine/system/CameraSystem.h" 
#include "GNEngine/system/InputToAccelerationSystem.h"
#include "GNEngine/system/PlayerAnimationControlSystem.h"
#include "GNEngine/system/FadeSystem.h"

/* --- Include All Components to use --- */
#include "GNEngine/component/SoundComponent.h"
#include "GNEngine/component/CameraComponent.h"
#include "GNEngine/component/RenderComponent.h"
#include "GNEngine/component/AccelerationComponent.h"
#include "GNEngine/component/VelocityComponent.h"
#include "GNEngine/component/InputControlComponent.h"
#include "GNEngine/component/TransformComponent.h"
#include "GNEngine/component/AnimationComponent.h"
#include "GNEngine/component/TextComponent.h"
#include "GNEngine/component/FadeComponent.h"
#include "GNEngine/component/PlayerAnimationControllerComponent.h"
#include "GNEngine/component/PlayerMovementComponent.h"

/* --- Include All Scenes to use --- */
#include "scene/LogoScene.h"
#include "scene/MainMenuScene.h"
#include "scene/TestScene.h"


Application::Application() {}
Application::~Application() {
    std::cerr << "App is successfully quited.\n";
}

int Application::init(){
    /* Initialize SDL Systems*/
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return -1;
    }
    
    /* Init Managers. (Singleton pattern) */
    auto& fileManager = FileManager::getInstance();
    fileManager.init(static_cast<std::filesystem::path>(APP_ROOT_PATH) / "config.bin");
    
    windowWidth = std::stoi(fileManager.getSetting("windowWidth", "1920"));
    windowHeight = std::stoi(fileManager.getSetting("windowHeight", "1080"));

    window_ = SDL_CreateWindow("Application", windowWidth, windowHeight, 0);
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if(!window_ || !renderer_){
        SDL_Log("Error occured in SDL_CreateWindow or SDL_CreateRenderer : %s", SDL_GetError());
        return -1;
    }

    /* Set additional settings */ 
    SDL_SetRenderVSync(renderer_, true); /* Enable VSync */


    /* ※Do not change the order of declarations.※ */
    /* (The order of declaration is the same as the order of destruction.) */
    /* --- Initialize all manager --- */
    GNManager::getInstance(renderer_, window_); // Initialize the singleton instance
    auto& entityManager = GNManager::getInstance().getEntityManager();
    auto& eventManager = GNManager::getInstance().getEventManager();
    auto& inputManager = GNManager::getInstance().getInputManager();
    auto& soundManager = GNManager::getInstance().getSoundManager();
    auto& textureManager = GNManager::getInstance().getTextureManager();
    auto& textManager = GNManager::getInstance().getTextManager();
    auto& animationManager = GNManager::getInstance().getAnimationManager();
    auto& fadeManager = GNManager::getInstance().getFadeManager();
    auto& systemManager = GNManager::getInstance().getSystemManager();
    auto& sceneManager = GNManager::getInstance().getSceneManager();
    auto& renderManager = GNManager::getInstance().getRenderManager();

    SDL_Rect viewport = {0, 0, windowWidth, windowHeight};
    GNManager::getInstance().getRenderManager().setViewport(viewport);

    /* --- Regist all systems --- */
    systemManager.registerSystem<RenderSystem>(SystemPhase::RENDER, renderManager, textManager);
    systemManager.registerSystem<InputSystem>(SystemPhase::PRE_UPDATE, eventManager, entityManager);
    systemManager.registerSystem<PlayerAnimationControlSystem>(SystemPhase::LOGIC_UPDATE, animationManager, textureManager, renderManager);
    systemManager.registerSystem<SoundSystem>(SystemPhase::LOGIC_UPDATE, soundManager);
    systemManager.registerSystem<CameraSystem>(SystemPhase::POST_UPDATE, renderManager);
    systemManager.registerSystem<AnimationSystem>(SystemPhase::POST_UPDATE);
    systemManager.registerSystem<InputToAccelerationSystem>(SystemPhase::PRE_UPDATE, eventManager, entityManager);
    systemManager.registerSystem<MovementSystem>(SystemPhase::PHYSICS_UPDATE);
    systemManager.registerSystem<FadeSystem>(SystemPhase::LOGIC_UPDATE, renderManager);
    
    /* --- Regist all Conpontnt to use --- */
    entityManager.registerComponentType<RenderComponent>();
    entityManager.registerComponentType<AnimationComponent>();
    entityManager.registerComponentType<SoundComponent>();
    entityManager.registerComponentType<TextComponent>();
    entityManager.registerComponentType<TransformComponent>();
    entityManager.registerComponentType<VelocityComponent>();
    entityManager.registerComponentType<FadeComponent>();
    entityManager.registerComponentType<AccelerationComponent>();
    entityManager.registerComponentType<PlayerAnimationControllerComponent>();
    entityManager.registerComponentType<InputControlComponent>();
    entityManager.registerComponentType<CameraComponent>();


    /* --- Regist all scane ---*/
    sceneManager.addScene("LogoScene", std::make_unique<LogoScene>(entityManager, sceneManager, eventManager, renderManager, soundManager, textureManager, animationManager, fadeManager));
    sceneManager.addScene("TestScene", std::make_unique<TestScene>(entityManager, eventManager, renderManager, soundManager, textureManager, textManager, animationManager));
    //sceneManager.addScene("MainMenuScene", std::make_unique<MainMenuScene>());

    sceneManager.loadScene("LogoScene");
    sceneManager.changeScene("LogoScene");
    // sceneManager.changeScene("TestScene");

    lastFrameTime_ = std::chrono::high_resolution_clock::now();
    return 0;
}

void Application::quit() {
    auto& fileManager = FileManager::getInstance();
    const std::filesystem::path configBinPath = static_cast<std::filesystem::path>(PROJECT_ROOT_PATH) /"app/data/config.bin";
    fileManager.saveSettings(configBinPath);
    //fileManager.saveLogs();

    // // Get current window size and save it
    // SDL_GetWindowSize(window_, &windowWidth, &windowHeight);
    // fileManager.setSetting("WindowWidth", std::to_string(windowWidth));
    // fileManager.setSetting("WindowHeight", std::to_string(windowHeight));
    // fileManager.saveSettings(configPath);

    std::cout << "Application - Cleaning up and quitting... " << std::endl;

    /* All Manager need no destruction */
    // gnManager_ will be destroyed automatically when Application goes out of scope.

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
    auto& systemManager = GNManager::getInstance().getSystemManager();
    auto& inputManager = GNManager::getInstance().getInputManager();
    auto& renderManager = GNManager::getInstance().getRenderManager();
    auto& sceneManager = GNManager::getInstance().getSceneManager();

    isRunning_ = true;
    while(isRunning_){
        // std::cerr << "[DEBUG] Application::run() - Top of the main loop.\n";
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime_).count();
        lastFrameTime_ = currentTime;

        // To prevent logic explosion by large deltaTime, clamp it.
        deltaTime = std::min(deltaTime, 0.1f);

        /* Process all events */
        if(!inputManager.processEvents()){
            // std::cerr << "[DEBUG] Application::run() - processEvents() returned false. Exiting loop.\n";
            isRunning_ = false;
            break;
        }
        inputManager.updateKeyStates();
        
        
        renderManager.clear();
        /*
        * SystemManager perform in the order. {PRE_UPDATE, LOGIT_UPDATE, PHYSICS_UPDATE, POST_UPDATE, RENDER}
        */
       // std::cerr << "[DEBUG] Application::run() - Calling systemManager_->updateAll().\n";
       systemManager.updateAll(deltaTime);
       // std::cerr << "[DEBUG] Application::run() - Finished systemManager_->updateAll().\n";
       
       // std::cerr << "[DEBUG] Application::run() - Calling sceneManager_->update()\n";
       sceneManager.update(deltaTime);
        
        renderManager.present();
    }
}

