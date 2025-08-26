#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Application.h"

// SDL_RunApp???몄텧??肄쒕갚 ?⑥닔
int SDLCALL myAppCallback(int argc, char* argv[]) {
    Application application;

    if(application.init() != 0){
        SDL_Log("Application initialize. code:%d: %s", SDL_GetError());
        return -1;
    }
    
    application.run(); /* main loop */

    application.quit();
    
    return 0;
}

int main(int argc, char* argv[]) {
    std::cout << "Acess main" << std::endl;
    const int result = SDL_RunApp(argc, argv, myAppCallback, nullptr);

    if (result != 0 ) {
        SDL_Log("Application exited with errors : %s", SDL_GetError());
    }
    else {
        std::cout << "Application exited successfully!" << std::endl;
    }

    return result;
}

