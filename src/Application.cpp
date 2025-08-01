#include "Application.h"
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
    renderManager_ = std::make_unique<RenderManager>(renderer_, window_);
    inputManager_ = std::make_unique<InputManager>(*eventManager_);
    textureManager_ = std::make_unique<TextureManager>(renderer_);
    textManager_ = std::make_unique<TextManager>(renderer_);
    soundManager_ = std::make_unique<SoundManager>();
    
    sceneManager_ = std::make_unique<SceneManager>(
        eventManager_.get(),
        renderManager_.get(),
        textureManager_.get(),
        soundManager_.get()
    );

    // //     // TestMp3.mp3 파일 로드 및 재생
    // std::filesystem::path mp3Path = std::filesystem::path(SOUND_ASSET_ROOT_PATH) / "TestMp3.mp3"; // Use std::filesystem::path
    // if (soundManager_->loadSound(mp3Path)) {
    //     soundManager_->playSound(mp3Path, SoundPriority::CRITICAL, 1.0f, 1.0f, true);
    //     std::cout << "TestMp3.mp3 loaded and playing." << std::endl;
    // } else {
    //     std::cerr << "Failed to load " << mp3Path.string() << "." << std::endl; // Use mp3Path.string()
    // }

    // TestScene 등록 및 전환
    sceneManager_->addScene("TestScene", std::make_unique<TestScene>(*eventManager_, *renderManager_, *textureManager_, *soundManager_));
    sceneManager_->changeScene("TestScene");

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

        /* Process Input */
        if(!inputManager_->processEvents()){
            isRunning_ = false;
            break;
        }

        /* Update */
        inputManager_->updateKeyStates();
        sceneManager_->update(0.016f); // deltaTime은 나중에 정확하게 계산하도록 수정 필요

        /* Render */
        renderManager_->clear();
        sceneManager_->render(renderer_);
        renderManager_->present();
    }
}