#include "TextureManager.h"
#include <iostream>

/*The Texture object.
Argument: */
TextureManager::TextureManager(){}

/* Destructor */
TextureManager::~TextureManager() {
    // unique_ptr will handle deallocation, so no explicit clear() needed for memory management
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
bool TextureManager::loadTexture(const std::string& filePath, float x, float y, float scaleX, float scaleY){
    UniqueSDL_Surface tmpSurface = nullptr;
    std::string fileExtention = filePath.substr(filePath.size()-4, filePath.size());

    /* Check if the file extension is ".bmp" or ".png" and create SDL_Surface*/
    if(fileExtention == ".bmp"){
        tmpSurface.reset(SDL_LoadBMP((IMAGE_ASSET_ROOT_PATH + filePath).c_str()));
        if(!tmpSurface){
            SDL_Log("TextureManager [%p] : SDL_LoadBMP failed: %s", static_cast<void*>(this), SDL_GetError());
            return false;
        }
    }
    else if(fileExtention == ".png"){
        tmpSurface.reset(IMG_Load((IMAGE_ASSET_ROOT_PATH + filePath).c_str()));
        if(!tmpSurface){
            SDL_Log("TextureManager [%p] : IMG_Load failed: %s", static_cast<void*>(this), SDL_GetError());
            return false;
        }
    }
    else {
        SDL_Log("TextureManager [%p] : Unsupported file format: %s", static_cast<void*>(this), (IMAGE_ASSET_ROOT_PATH + filePath).c_str());
        return false;
    }

    /* create texture */
    SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(rawRenderer_, tmpSurface.get());
    if(!sdlTexture){
        SDL_Log("Error occured in texture: %s", SDL_GetError());
        return false;
    }   

    // Create a new Texture object and move the SDL_Texture into it
    SDL_FRect dstRect = {x, y, (float)tmpSurface->w * scaleX, (float)tmpSurface->h * scaleY};
    textureMap_[filePath] = std::make_unique<Texture>(sdlTexture, dstRect, scaleX, scaleY);

    count_++; /* Increase num of the texture*/
    return true;
}

bool TextureManager::renderTexture(const std::string& filePath){
    auto it = textureMap_.find(filePath);
    if (it == textureMap_.end()) {
        SDL_Log("TextureManager [%p] : Texture not found for rendering: %s", this, filePath.c_str());
        return false;
    }
    if(!SDL_RenderTexture(rawRenderer_, it->second->getSDLTexture(), NULL, &it->second->getDstRect())){
        SDL_Log("TextureManager [%p] : Error is occured in SDL_RenderTexture: %s", this, SDL_GetError());
        return false;
    }
    return true;
}


Texture* TextureManager::getTexture(const std::string& filePath){
    auto it = textureMap_.find(filePath);
    if (it == textureMap_.end()) {
        return nullptr; // Texture not found
    }
    return it->second.get(); // Return raw pointer, ownership remains with manager
}

void TextureManager::moveTexture(const std::string& fileName, float x, float y) {
    auto it = textureMap_.find(fileName);
    if (it != textureMap_.end()) {
        it->second->getDstRect().x += x;
        it->second->getDstRect().y += y;
    }
}

/* return position. if position is 'x' -> return x or 'y' -> return y or 'w' -> return width or 'h' -> return height.*/
float TextureManager::getPosition(const std::string& fileName, char position){
    auto it = textureMap_.find(fileName);
    if (it == textureMap_.end()) {
        return 0.0f; // Or throw an exception, or log an error
    }

    if(position == 'x') {return it->second->getDstRect().x;}
    else if(position == 'y') {return it->second->getDstRect().y;}
    else if(position == 'w') {return it->second->getDstRect().w;}
    else if(position == 'h') {return it->second->getDstRect().h;}
    return 0.0f; // Invalid position character
}

void TextureManager::rotateTexture(const std::string& fileName, float angle){
    //SDL_RenderTextureRotated(rawRenderer_, texture, NULL, &dstRect, {float(dstRect.x/2), float(dstRect.y/2)}, angle, 0);  
}

bool TextureManager::loadBackground(const std::string& filePath, int windowWidth, int windowHeight, float scaleX, float scaleY) { 
    // This function currently calls loadTexture, which adds to textureMap_.
    // If backgrounds are truly separate, they should be added to backgroundMap_.
    // For now, I'll assume it's okay to load into textureMap_ and then move to backgroundMap_ if needed.
    // A better approach might be to have a single load function that takes a map reference.
    bool success = loadTexture(filePath, (float)windowWidth, (float)windowHeight, scaleX, scaleY);
    if (success) {
        // If you want to move it to backgroundMap_ and remove from textureMap_:
        // backgroundMap_[filePath] = std::move(textureMap_[filePath]);
        // textureMap_.erase(filePath);
    }
    return success;
}
bool TextureManager::renderBackground(const std::string& fileName){
    auto it = backgroundMap_.find(fileName);
    if (it == backgroundMap_.end()) {
        SDL_Log("TextureManager [%p] : Background texture not found for rendering: %s", this, fileName.c_str());
        return false;
    }
    SDL_RenderTexture(rawRenderer_, it->second->getSDLTexture(), NULL, &it->second->getDstRect());
    return true;
}