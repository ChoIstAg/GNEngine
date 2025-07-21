#include "Application.h"
#include <iostream>

Application::Application() : inputManager_(eventManager_) {};

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

    /* 매니저 초기화*/
    eventManager_.init();
    inputManager_.init();
    textureManager_.init(renderer_);

    return 0;
}

void Application::quit() {
    std::cout << "cleaning up and quitting... " << std::endl;
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    SDL_Quit();
}