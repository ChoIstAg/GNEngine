#include "TextureManager.hpp"
#include <iostream>

/*The Texture object.
Argument: */
TextureManager::TextureManager(){}

/* Destructor */
TextureManager::~TextureManager() {
    // Free objects
    textureMap_.clear();
    std::cerr << "TextureManager " << this << " is successfully destroyed" << std::endl;
}

/* Initialize Texture and load texture if preLoad is True */
bool TextureManager::initialize(SDL_Renderer* rawRenderer){
    textureMap_.clear();

    if(!rawRenderer){ 
        SDL_Log("rawRenderer in TextureManager is erroneous: %s", SDL_GetError());
        return false;
    }
    rawRenderer_ = rawRenderer;

    return true;
}   

/* Loating texture(bmp, png) or animation(gif, png(sprite sheet).*/
bool TextureManager::loadTexture(std::string filePath, float x, float y, float scaleX, float scaleY){
    SDL_Surface* tmpSurface;
    std::string fileExtention = filePath.substr(filePath.size()-4, filePath.size());

    /* Check if the file extension is ".bmp" or ".png" and create SDL_Surface*/
    if(fileExtention == ".bmp"){
        tmpSurface = SDL_LoadBMP((IMAGE_ASSET_ROOT_PATH + filePath).c_str());
        if(tmpSurface == NULL){
            SDL_Log("TextureManager [%p] : SDL_LoadBMP failed: %s", static_cast<void*>(this), SDL_GetError());
            return false;
        }
    }
    else if(fileExtention == ".png"){
        tmpSurface = IMG_Load((IMAGE_ASSET_ROOT_PATH + filePath).c_str());
        if(tmpSurface == nullptr){
            SDL_Log("TextureManager [%p] : IMG_Load failed: %s", static_cast<void*>(this), SDL_GetError());
            return false;
        }
    }
    else {
        SDL_Log("TextureManager [%p] : Unsupported file format: %s", static_cast<void*>(this), (IMAGE_ASSET_ROOT_PATH + filePath).c_str());
        return false;
    }

    /* create texture */
    textureMap_[filePath] = new Texture{
        SDL_CreateTextureFromSurface(rawRenderer_, tmpSurface), /* SDL_Texture**/
        {x, y, tmpSurface->w * scaleX, tmpSurface->h * scaleY}, /* SDL_FRect*/
        scaleX, scaleY
    }; /* scales for width, height*/
    if(textureMap_[filePath] == nullptr){
        SDL_Log("Error occured in texture: %s", SDL_GetError());
        return false;
    }   

    count_++; /* Increase num of the texture*/
    return true;
}

bool TextureManager::renderTexture(std::string filePath){
    if(!SDL_RenderTexture(rawRenderer_, textureMap_[filePath]->texture, NULL, &textureMap_[filePath]->dstRect)){
        SDL_Log("TextureManager [%p] : Error is occured in SDL_RenderTexture: %s", this, SDL_GetError());
        return false;
    }
    return true;
}


Texture* TextureManager::getTexture(std::string filePath){
    return textureMap_[filePath];
}

void TextureManager::moveTexture(std::string fileName, float x, float y) {
    textureMap_[fileName]->dstRect.x += x;
    textureMap_[fileName]->dstRect.y += y;
}

/* return position. if position is 'x' -> return x or 'y' -> return y or 'w' -> return width or 'h' -> return height.*/
float TextureManager::getPosition(std::string fileName, char position){
    if(position == 'x') {return textureMap_[fileName]->dstRect.x;}
    else if(position == 'y') {return textureMap_[fileName]->dstRect.y;}
    else if(position == 'w') {return textureMap_[fileName]->dstRect.w;}
    else if(position == 'h') {return textureMap_[fileName]->dstRect.h;}
    return false;
}

void TextureManager::rotateTexture(std::string fileName, float angle){
    //SDL_RenderTextureRotated(rawRenderer_, texture, NULL, &dstRect, {float(dstRect.x/2), float(dstRect.y/2)}, angle, 0);  
}

bool TextureManager::loadBackground(std::string filePath, int windowWidth, int windowHeight, float scaleX, float scaleY) { 
    loadTexture(filePath, int(windowWidth), int(windowHeight), scaleX, scaleY);
    return true;
}
bool TextureManager::renderBackground(std::string fileName){
    SDL_RenderTexture(rawRenderer_, backgroundMap_[fileName]->texture, NULL, &backgroundMap_[fileName]->dstRect);
    return true;
}