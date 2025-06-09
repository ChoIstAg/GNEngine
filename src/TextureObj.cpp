
#include "TextureObj.hpp"
#include <iostream>

TextureObj::TextureObj(SDL_Renderer* renderer, std::string filePath, bool preLoad, float x, float y, float scaleX, float scaleY) 
: renderer(renderer), filePath(filePath), preLoad(preLoad), x(x), y(y), scaleX(scaleX), scaleY(scaleY){

    std::cout << &renderer << std::endl;
    if(preLoad == 1){ /*preLoad가 true일 때만 자동으로 로딩딩*/
        std::cout << this << "::load_texture()" << std::endl;
        if(!load_texture()){ /* load failure */
            SDL_Log("load_texture failed: %s", SDL_GetError());
        }

        else{
            std::cout << "load complete" << std::endl;
        }
    }
}

TextureObj::~TextureObj() {
    // Free objects
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
    std::cout << "Texture destroyed" << std::endl;
}


bool TextureObj::load_texture(){

    /* Check if the file extension is ".bmp" or ".png" and create SDL_Surface*/
    if(filePath.substr(filePath.size() - 4, 4) == ".bmp"){
        surface = SDL_LoadBMP(filePath.c_str());
        if(surface == NULL){
            SDL_Log("SDL::SDL_LoadBMP failed: %s", SDL_GetError());
            return false;
        }

    }
    else if(filePath.substr(filePath.size() - 4, 4) == ".png"){
        surface = IMG_Load(filePath.c_str());
        if(surface == NULL){
            SDL_Log("SDL_Image::IMG_Load failed: %s", SDL_GetError());
            return false;
        }
    }
    else {
        SDL_Log("Unsupported file format: %s", filePath.c_str());
        return false;
    }
    std::cout << filePath << std::endl;

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == nullptr){
        SDL_Log("Error occured in texture: %s", SDL_GetError());
    }


    SDL_GetTextureSize(texture, &width, &height);
    dstRect = {x, y, width * scaleX, height * scaleY};
    if(dstRect.w == 0 || dstRect.h == 0){
        SDL_Log("Error occured in dstRect: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool TextureObj::render_texture(){
    if(SDL_RenderTexture(renderer, texture, NULL, &dstRect)) {return 1;}
    else {return 0;}
}

SDL_Texture* TextureObj::get_texture(){
    return texture;
}

void TextureObj::move_texture(float x, float y) {
    dstRect.x += x;
    dstRect.y += y;
}

/* return position. if position is 'x' -> return x or 'y' -> return y or 'w' -> return width or 'h' -> return height.*/
float TextureObj::get_position(char position){
    if(position == 'x') {return dstRect.x;}
    if(position == 'y') {return dstRect.y;}
    if(position == 'w') {return dstRect.w;}
    if(position == 'h') {return dstRect.h;}

    return -1;
}

void TextureObj::turn_texture(float angle) {
    //SDL_RenderTextureRotated(renderer, texture, NULL, &dstRect, {float(dstRect.x/2), float(dstRect.y/2)}, angle, 0);  
}  

void TextureObj::cout_test(){
    std::cout << "바보" << std::endl;
}