/*
    윈도우에서 창을 생성 한 후 
    (텍스트 파일 내용 불러오기(fstream, sstream), 폰트파일 불러오기(SDL3_ttf) 사용)
*/

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <iostream>
#include <print>
#include "TextureObj.hpp"

#define PROJECT_ROOT_PATH "C:/Development/code/project/Noma's Home/"
constexpr std::string_view project_root_path = PROJECT_ROOT_PATH;


class AppState{
    public:
    int windowWidth = 1280;
    int windowHeight = 720;
    SDL_FRect viewport = {0, 0, static_cast<float>(windowWidth), static_cast<float>(windowHeight)};
    
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;
    
    ~AppState(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);    
        if (texture) {SDL_DestroyTexture(texture);}
        if (surface) {SDL_DestroySurface(surface);}
    }
};
static AppState *appState = new AppState();


class TextManager{
    public:
    TTF_Font* font = nullptr;
    int fontSize = 11;
    SDL_Color fontColor = {255, 255, 255, 255}; 
    
    SDL_Surface* textSurface = nullptr;
    SDL_Texture* textTexture = nullptr;
    SDL_FRect dstRect;
    
    std::vector<std::string> text;

    ~TextManager(){
        if(font) {TTF_CloseFont(font);}
        if(textSurface) {SDL_DestroySurface(textSurface);}
        if(textTexture) {SDL_DestroyTexture(textTexture);}
    }

    std::string load_textfile(const std::string& filepath){
        std::ifstream file(filepath);
        if(!file.is_open()) {
            std::cerr << "파일을 열 수 없습니다: " << filepath << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};
static TextManager *textManager = new TextManager();

class TextureManager{ /* 텍스처를 총괄 */
public:

    ~TextureManager(){
        /* 초기화*/
    }
};
TextureManager *textureManager = new TextureManager;


/* APP initialization*/
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
    if(!appState){
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("노마의 집집", appState->windowWidth, appState->windowHeight, SDL_WINDOW_RESIZABLE, &appState->window, &appState->renderer)){
        return SDL_APP_FAILURE;
    }
    
    bool ret = TTF_Init(); /*initalize SDL_ttf*/
    if(ret){
        SDL_Log("TTF_Init() error: %s", SDL_GetError());
    }   

    /* Open font*/
    std::string fontPath1 = std::string(project_root_path) + std::string("asset/font/CookieRun Black.ttf");
    textManager->font = TTF_OpenFont(fontPath1.c_str(), textManager->fontSize);
    if(!textManager->font){
        SDL_Log("font error: %s", SDL_GetError());
    }
    
    /*load txt file*/
    textManager->text.push_back(textManager->load_textfile(PROJECT_ROOT_PATH + std::string("asset/text/content.txt")));
    //std::cout << textManager->text[0] << std::endl;
    //std::print("text[0].length = {}", textManager->text[0].length());

    /* convert font to surface and texture*/
    textManager->textSurface = TTF_RenderText_Blended_Wrapped(textManager->font, textManager->text[0].c_str(), textManager->text[0].length(), {255, 255, 255, 255}, appState->windowWidth);
    textManager->textTexture = SDL_CreateTextureFromSurface(appState->renderer, textManager->textSurface);
    textManager->dstRect = {0.0f, 0.0f, float(textManager->text[0].length() * textManager->fontSize), float(textManager->fontSize)};
    
    /* create texture*/
    
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate){
    SDL_RenderClear(appState->renderer); /*screen refrash*/


    SDL_RenderTexture(appState->renderer, textManager->textTexture, NULL, &appState->viewport);
    

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

            if(event->key.key == SDLK_1){ /* 앞장으로 넘기기 */

            }
            if(event->key.key == SDLK_2){ /* 뒷장으로 넘기기 */
                
            }

            if(event->key.key == SDLK_RIGHT){
                appState->viewport.x += 100;
            }
            if(event->key.key == SDLK_LEFT){
                appState->viewport.y -= 100;
            }
    }
    return SDL_APP_CONTINUE; 
}    


/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result){
    delete appState;
    delete textManager;
    TTF_Quit();
}   