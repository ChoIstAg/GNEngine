
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

/* We will use this renderer to draw into this window every frame. */
struct AppState {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

AppState *app_state = new AppState(); /*allocate AppState */
//AppState *app_state = (AppState *)SDL_calloc(1, sizeof(AppState)); //same as above


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
    
    if (!app_state) {
        return SDL_APP_FAILURE;
    }


    if (!SDL_CreateWindowAndRenderer("example", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &app_state->window, &app_state->renderer)) {  //create a window and render
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    
    return SDL_APP_CONTINUE; 
}    

SDL_Vertex verts[3] = {
    {{100,100},{255,0,0,0},{0,0}},
    {{200,100},{0,255,0,0},{0,0}},
    {{150,500},{0,0,255,0},{0,0}}
};
void mooving(float x, float y) {
    // Update the position and size of the texture based on the parameters
    for(auto& vert:verts){
        vert.position.x += x;
        vert.position.y += y;
    }
}
void decrease_alpha(float alpha) {
    // Update the alpha value of the texture based on the parameter
    for(auto& vert:verts){
        vert.color.a += alpha;
    }
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate){
    SDL_RenderClear(app_state->renderer); /*screen refrash*/
    
    
    /*...*/
    SDL_RenderGeometry(app_state->renderer, NULL, verts, 3, NULL, 0);


    SDL_RenderPresent(app_state->renderer); /*screen update*/
    return SDL_APP_CONTINUE;
}    

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    
    switch(event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS; /* Successful termination */
        case SDL_EVENT_KEY_DOWN:
            if(event->key.key == SDLK_RIGHT){
                mooving(100.0f, 0.0f);
            }
            if(event->key.key == SDLK_LEFT){
                mooving(-100.0f, 0.0f);
             }
            if(event->key.key == SDLK_UP){
                mooving(0.0f, -100.0f);
            }    
            if(event->key.key == SDLK_DOWN){
                mooving(0.0f, 100.0f);
            }
            if(event->key.key == SDLK_SPACE){
                decrease_alpha(-5.0f);
            }
    }

    return SDL_APP_CONTINUE; 
}    


/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result){

}