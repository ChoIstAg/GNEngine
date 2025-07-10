#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "MainCallback.h"

int main(int argc, char* argv[]) {
    
    const int result = SDL_RunApp(argc, argv, mainCallback, nullptr);

    if (result != 0 ) {
        SDL_Log("Application exited with errors : %s", SDL_GetError());
    }
    else {
        std::cout << "Application exited successfully!" << std::endl;
    }

    return result;
}