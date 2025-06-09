
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static int WINDOW_WIDTH = 1280;
static int WINDOW_HEIGHT = 720;

/* We will use this renderer to draw into this window every frame. */
struct AppState {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};

static AppState *app_state = new AppState(); /*allocate AppState */
//AppState *app_state = (AppState *)SDL_calloc(1, sizeof(AppState)); //same as above


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
    
    if (!app_state) {
        return SDL_APP_FAILURE;
    }


    if (!SDL_CreateWindowAndRenderer("example", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &app_state->window, &app_state->renderer)) {  //create a window and render
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

     return SDL_APP_CONTINUE; 
}    

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate){
    SDL_RenderClear(app_state->renderer); /*screen refrash*/


    /*...*/


    SDL_RenderPresent(app_state->renderer); /*screen update*/
    return SDL_APP_CONTINUE;
}    

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    


    return SDL_APP_CONTINUE; 
}    


/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result){

}