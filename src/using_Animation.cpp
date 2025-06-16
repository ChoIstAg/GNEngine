
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <iostream>

#define PROJECT_ROOT_PATH "C:/Development/code/project/noma's home/"

/* We will use this renderer to draw into this window every frame. */
class AppState{
    public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    int windowWidth = 1280;
    int windowHeight = 720;
    SDL_FRect viewport = {0, 0, static_cast<float>(windowWidth), static_cast<float>(windowHeight)};
    
    Uint64 lastTime = 0;

    ~AppState(){
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
    }
};
AppState *appState = new AppState(); /*allocate AppState */

class TextureManager{
public:
    IMG_Animation* animation; 
    std::vector<SDL_Texture*> frameTextures;
    float x, y, width, height, scaleX, scaleY;
    int correntFrame = 0;

    TextureManager(bool preLoad, float scaleX, float scaleY)
    : scaleX(scaleX), scaleY(scaleY){
        if(preLoad == true){
            loading();
            width = animation->w * scaleX;
            height = animation->h * scaleY;
        }
    }

    bool loading(){
        animation = IMG_LoadAnimation(std::string(std::string(PROJECT_ROOT_PATH) + "asset/image/catrun.gif").c_str());

        if(animation == 0){ //예외처리
            SDL_Log("animation generation is erronous: %s", SDL_GetError());
            return 0;
        }
        else{ textureAnimationLoading();}

        return 1;
    }

    bool textureAnimationLoading(){ // animation 로딩 후 사용
        for (int i = 0; i < animation->count; i++) { /* surface를 texture로 변환*/
            SDL_Texture* texture = SDL_CreateTextureFromSurface(appState->renderer, animation->frames[i]);
            frameTextures.push_back(texture);
        }

        std::cout << "loading complete in texture" << std::endl;
        return 1;
    }
};
TextureManager *textureManager = new TextureManager(1, 5, 5);

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
    if (!appState) {
        SDL_Log("Couldn't create appState: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("using Animation", appState->windowWidth, appState->windowHeight, 0, &appState->window, &appState->renderer)) {  //create a window and render
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    for(int i = 0; i < textureManager->animation->count; i++){
        std::cout << textureManager->animation->delays[i] << std::endl;
    }
    
    std::cout << "loading complete" << std::endl;
    return SDL_APP_CONTINUE; 
}    

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate){
    SDL_RenderClear(appState->renderer); /*screen refrash*/


    Uint64 now = SDL_GetTicks();
    // if (now - appState->lastTime >= textureManager->animation->delays[textureManager->animation->count]) {
    //     textureManager->correntFrame = (textureManager->correntFrame + 1) % textureManager->animation->count;
    //     appState->lastTime = now;
    // }
    textureManager->correntFrame = (textureManager->correntFrame + 1) % textureManager->animation->count;
    appState->lastTime = now;

    std::cout << "corrent frame: "<< textureManager->correntFrame << std::endl;

    SDL_FRect dst = {100.0f, 100.0f, textureManager->width, textureManager->height};
    if(!SDL_RenderTexture(appState->renderer, textureManager->frameTextures[textureManager->correntFrame], nullptr, &dst)){
        SDL_Log("error occured in SDL_RenderTexture: %s", SDL_GetError());
    }


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