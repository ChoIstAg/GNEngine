#include "Application.h"
#include <iostream>

Application::Application() = default;

int Application::init(){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return -1;
    }

    window_ = SDL_CreateWindow("Text main callback", windowWidth, windowHeight, 0);
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if(!window_ || !renderer_){
        SDL_Log("Error occured in SDL_CreateWindow : %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    textureManager.initialize(renderer_);
    return 0;
}

void Application::quit() {
    std::cout << "cleaning up and quitting... " << std::endl;
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    SDL_Quit();
}