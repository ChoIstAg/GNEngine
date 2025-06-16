
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


/* We will use this renderer to draw into this window every frame. */
class AppState {
public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    int windowWidth = 1280;
    int windowHeight = 720;
    SDL_FRect viewport = {0, 0, static_cast<float>(windowWidth), static_cast<float>(windowHeight)};

    ~AppState(){
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
    }
};
static AppState *appState = new AppState(); /*allocate AppState */


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
    if (!appState) {
        SDL_Log("Couldn't create appState: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }


    if (!SDL_CreateWindowAndRenderer("template", appState->windowWidth, appState->windowHeight, 0, &appState->window, &appState->renderer)) {  //create a window and render
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

     return SDL_APP_CONTINUE; 
}    

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate){
    SDL_RenderClear(appState->renderer); /*screen refrash*/


    /*...*/


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
            if(event->key.key == SDLK_ESCAPE) { return SDL_APP_SUCCESS; }

    }

    
    return SDL_APP_CONTINUE; 
}    


/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result){
    delete appState;
}