#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "TextureManager.hpp"

/* We will use this renderer to draw into this window every frame. */
struct AppState {
    int windowWidth = 1280;
    int windowHeight= 720;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};
static AppState* appState = new AppState; /*allocate AppState */


// TextureManager* TextureManager1 = nullptr; // Remove or comment out this line if not needed yet, or construct with correct arguments when needed
TextureManager* textureManager = new TextureManager;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
    
    if (!appState) {
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("example", appState->windowWidth, appState->windowHeight, SDL_WINDOW_RESIZABLE, &appState->window, &appState->renderer)) {  //create a window and render
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    textureManager->initialize(appState->renderer);
    textureManager->loadTexture("example_png.png", 0, 0, 1, 1);

     return SDL_APP_CONTINUE; 
}    

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate){
    SDL_RenderClear(appState->renderer); /*screen refrash*/


    textureManager->renderTexture("example_png.png");


    SDL_RenderPresent(appState->renderer); /*screen update*/
    return SDL_APP_CONTINUE;
}    

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    
    switch(event->type){
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
            break;
        case SDL_EVENT_KEY_DOWN:
            if(event->key.key == SDLK_ESCAPE){ return SDL_APP_SUCCESS; }
            if(event->key.key == SDLK_1) { textureManager->moveTexture("example_png.png", 100.0f, 0.0f); }
            if(event->key.key == SDLK_2) { textureManager->moveTexture("example_png.png", -100.0f, 0.0f); }
    }

    return SDL_APP_CONTINUE; 
}    

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result){
    delete appState;
}