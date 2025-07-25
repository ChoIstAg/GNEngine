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

    textManager_ = std::make_unique<TextManager>(renderer_);

    std::string fontPath = std::string(FONT_ASSET_ROOT_PATH) + "CookieRun Regular.ttf";
    textManager_->loadFont("cookie-run", fontPath, 24);

    testText_ = std::make_unique<TextObject>(
        textManager_->createText("cookie-run", "Hello, GNEngine!", {255, 255, 255, 255}),
        100.0f, 100.0f
    );

    testObject_ = std::make_unique<TestObject>(eventManager_, textureManager_, renderManager_);
    
    return 0;
}

void Application::quit() {
    std::cout << "cleaning up and quitting... " << std::endl;
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    /* Manager들은 모두 자동으로 소멸함. */

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

        if (testObject_) {
            testObject_->update(); // unique_ptr이므로 -> 연산자 사용
        }

        if (testText_) {
            testText_->render();
        }

        renderManager_.present();
    }
}