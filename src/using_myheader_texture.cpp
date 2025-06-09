#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "TextureObj.hpp"

constexpr std::string_view PROJECT_PATH = "c:\\Development\\code\\project\\sdl3_test\\asset\\";

static int WINDOW_WIDTH = 1280;
static int WINDOW_HEIGHT = 720;

/* We will use this renderer to draw into this window every frame. */
class AppState {
public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_Rect viewPort;

    AppState(){
        viewPort = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    }
};

class TextureManager{
public:
    TextureObj* texture1 = nullptr;

    ~TextureManager(){
        delete texture1;
    }
};

static AppState *app_state = new AppState;
static TextureManager *texture_manager = new TextureManager;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
    if (!app_state) {
        SDL_Log("app_state is invalid: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("using my header file for rendering texture", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &app_state->window, &app_state->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //*app_state->viewPort = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};


    /* Initialize texture1 */
    texture_manager->texture1 = new TextureObj(app_state->renderer, std::string(PROJECT_PATH) + "example_bmp.bmp", 1, 100, 100, 1, 1);
    
    return SDL_APP_CONTINUE; 
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate){
    SDL_RenderClear(app_state->renderer); /* screen refresh */

    // Render texture1
    if (texture_manager->texture1) {
        texture_manager->texture1->render_texture();
    }

    SDL_SetRenderViewport(app_state->renderer, &app_state->viewPort);


    SDL_RenderPresent(app_state->renderer); /* screen update */
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    switch(event->type){
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS; /* Successful termination */

        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_ESCAPE) {
                return SDL_APP_SUCCESS; /* Successful termination */
            }
            if (event->key.key == SDLK_UP) {
                texture_manager->texture1->move_texture(0, -100); // Move up
            }
            if (event->key.key == SDLK_DOWN) {
                texture_manager->texture1->move_texture(0, 100); // Move down
            }
            if (event->key.key == SDLK_RIGHT) {
                texture_manager->texture1->move_texture(100, 0); // Move right
            }
            if (event->key.key == SDLK_LEFT) {
                texture_manager->texture1->move_texture(-100, 0); // Move left
            }
            if(event->key.key == SDLK_W){
                app_state->viewPort.y += 100;
            }
            if(event->key.key == SDLK_S){
                app_state->viewPort.y -= 100;
            }
            break;
    }

    return SDL_APP_CONTINUE; 
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result){
    
    delete texture_manager;

    /* Free app_state */
    delete app_state;

    SDL_Quit();
}