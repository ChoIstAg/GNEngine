#include <iostream>
#include "Maincallback.h"

#include "Application.h"

int SDLCALL mainCallback(int argc, char* argv[]){
    Application application;

    if(application.init() != 0){
        SDL_Log("Application initialize : %s", SDL_GetError());
        return -1;
    }
    
    bool quit;

    /* Main loop. */
    while (!quit) {
        SDL_RenderClear(application.getRenderer());

        application.inputManager.updateKeys();
        if(application.inputManager.isKeyDown(SDL_SCANCODE_1)) {
            std::cout << "Key pressed 1" << std::endl;
        }
        if(application.inputManager.isKeyUp(SDL_SCANCODE_1)) {
            std::cout << "Key Up 1" << std::endl;
        }

        SDL_RenderPresent(application.getRenderer());
    }

    application.quit();
    return 0;
}