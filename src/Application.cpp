#include "Application.h"
#include <iostream>

Application::Application() 
    : inputManager_(eventManager_)
{};

int Application::init(){
    
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0) { /* Initialize SDL Systems*/
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return -1;
    }
    
    window_ = SDL_CreateWindow("Text main callback", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (renderer_) {
        SDL_SetRenderVSync(renderer_, true);
    }
    if(!window_ || !renderer_){
        SDL_Log("Error occured in SDL_CreateWindow : %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    
    // RenderManager와 TextureManager 초기화
    renderManager_.init(renderer_, window_);
    textureManager_.init(renderer_);
    
    /* 매니저 초기화*/
    eventManager_.init();
    inputManager_.init();

    // BlankObject를 init()에서 초기화
    blankObject_ = std::make_unique<BlankObject>(eventManager_, textureManager_, renderManager_);
    
    return 0;
}

void Application::quit() {
    std::cout << "cleaning up and quitting... " << std::endl;
    // textureManager_와 renderManager_는 멤버 변수로 자동 소멸됩니다.
    // blankObject_도 unique_ptr이므로 자동으로 소멸됩니다.
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    SDL_Quit();
}

void Application::run() {
    isRunning_ = true;
    while(isRunning_){
        /* Process Input */
        if(!inputManager_.eventProcessing()){
            isRunning_ = false;
            break;
        }

        /* Update */
        inputManager_.updateKeyStates();

        /* Render */
        renderManager_.clear();

        if (blankObject_) {
            blankObject_->update(); // unique_ptr이므로 -> 연산자 사용
        }

        renderManager_.present();
    }
}