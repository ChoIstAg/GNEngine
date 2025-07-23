#include "TextureManager.h"
#include <iostream>
#include <SDL3_image/SDL_image.h>

TextureManager::TextureManager() : renderer_(nullptr) {}

TextureManager::~TextureManager() {
    textureMap_.clear();
    std::cerr << "TextureManager " << this << " is successfully destroyed" << std::endl;
}

bool TextureManager::init(SDL_Renderer* renderer){
    if(!renderer){ 
        SDL_Log("TextureManager::init - rawRenderer is null: %s", SDL_GetError());
        return false;
    }
    renderer_ = renderer;

    return true;
}

bool TextureManager::loadTexture(const std::string& filePath){
    // 이미 로드된 텍스처인지 확인
    if (textureMap_.count(filePath)) {
        std::cerr << "TextureManager::loadTexture - Texture already loaded: " << filePath << std::endl;
        return true; // 이미 로드되어 있다면 true 반환
    }

    SDL_Surface* tmpSurface = nullptr;
    std::string fullPath = IMAGE_ASSET_ROOT_PATH + filePath; /* 파일 절대 경로 */
    std::string fileExtension = filePath.substr(filePath.find_last_of('.') + 1); /* 파일 확장자 추출 */

    if (fileExtension == "bmp") {
        tmpSurface = SDL_LoadBMP(fullPath.c_str());
    } 
    else if (fileExtension == "png" || fileExtension == "jpg" || fileExtension == "jpeg" || fileExtension == "gif") {
        tmpSurface = IMG_Load(fullPath.c_str());
    } 
    else {
        SDL_Log("TextureManager::loadTexture - Unsupported file format for %s", fullPath.c_str());
        return false;
    }

    if (tmpSurface == nullptr) {
        SDL_Log("TextureManager::loadTexture - Failed to load surface %s: %s", fullPath.c_str(), SDL_GetError());
        return false;
    }

    
    SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(renderer_, tmpSurface);
    if (sdlTexture == nullptr) {
        SDL_Log("TextureManager::loadTexture - Failed to create texture from surface %s: %s", fullPath.c_str(), SDL_GetError());
        return false;
    }
    
    /* unique_ptr을 사용하여 Texture 객체 생성 및 맵에 저장 */
    textureMap_[filePath] = std::make_unique<Texture>(sdlTexture, tmpSurface->w, tmpSurface->h);
    
    SDL_DestroySurface(tmpSurface); // SDL_Surface 메모리 해제
    
    return true;
}

Texture* TextureManager::getTexture(const std::string& filePath) const {
    auto it = textureMap_.find(filePath);
    if (it != textureMap_.end()) {
        return it->second.get(); // unique_ptr에서 raw 포인터 반환
    }
    SDL_Log("TextureManager::getTexture - Texture not found: %s", filePath.c_str());
    return nullptr;
}