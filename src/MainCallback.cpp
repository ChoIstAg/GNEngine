#include <iostream>
#include "MainCallback.h"

#include "Application.h"
#include "engine/object/BlankObject.h"
#include "engine/event/eventManager.h"

int SDLCALL mainCallback(int argc, char* argv[]){
    Application application;

    if(application.init() != 0){
        SDL_Log("Application initialize : %s", SDL_GetError());
        return -1;
    }
    
    BlankObject blankObject(application.eventManager_);

    bool quit = false;

    /* Main loop. */
    while (!quit) {
        SDL_RenderClear(application.getRenderer());

        application.inputManager_.eventProcessing();
        application.inputManager_.updateKeyStates();
        

        SDL_RenderPresent(application.getRenderer());
    }

    application.quit();
    return 0;
}