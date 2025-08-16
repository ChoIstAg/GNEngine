#include "engine/manager/TextureManager.h"
#include <iostream>
#include <filesystem>
#include <SDL3_image/SDL_image.h>

TextureManager::TextureManager(SDL_Renderer* renderer) 
    : renderer_(renderer) {
        if(!renderer){ 
            SDL_Log("TextureManager::init - rawRenderer is null: %s", SDL_GetError());
        }
    }

TextureManager::~TextureManager() {
    textureMap_.clear();
    std::cerr << "TextureManager " << this << " is successfully destroyed" << std::endl;
}

bool TextureManager::loadTexture(const std::filesystem::path& filePath){
    // 이미 로드된 텍스처인지 확인
    if (textureMap_.count(filePath)) {
        return true; // 이미 로드되어 있다면 true 반환
    }

    SDL_Surface* tmpSurface = nullptr;
    
    // std::filesystem을 사용하여 확장자 추출
    std::string extension = filePath.has_extension() ? filePath.extension().string() : "";

    // 확장자를 소문자로 변환하여 비교 (예: .PNG, .JPG 등 처리)
    std::transform(extension.begin(), extension.end(), extension.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    if (extension == ".bmp") {
        tmpSurface = SDL_LoadBMP(filePath.string().c_str());
    } 
    else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".gif") {
        tmpSurface = IMG_Load(filePath.string().c_str());
    } 
    else {
        SDL_Log("TextureManager::loadTexture - Unsupported file format for %s", filePath.string().c_str());
        return false;
    }

    if (tmpSurface == nullptr) {
        SDL_Log("TextureManager::loadTexture - Failed to load surface %s: %s", filePath.string().c_str(), SDL_GetError());
        return false;
    }

    SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(renderer_, tmpSurface);
    if (sdlTexture == nullptr) {
        SDL_Log("TextureManager::loadTexture - Failed to create texture from surface %s: %s", filePath.string().c_str(), SDL_GetError());
        return false;
    }

    // 텍스처 스케일 모드를 Nearest Neighbor로 설정
    if (!SDL_SetTextureScaleMode(sdlTexture, SDL_SCALEMODE_NEAREST)) {
        SDL_Log("TextureManager::loadTexture - Failed to set texture scale mode for %s: %s", filePath.string().c_str(), SDL_GetError());
    }
    
    /* unique_ptr을 사용하여 Texture 객체 생성 및 맵에 저장 */
    textureMap_[filePath] = std::make_unique<Texture>(sdlTexture, tmpSurface->w, tmpSurface->h);
    
    SDL_DestroySurface(tmpSurface); // SDL_Surface 메모리 해제
    
    return true;
}

Texture* TextureManager::getTexture(const std::filesystem::path& filePath) const {
    auto it = textureMap_.find(filePath);
    if (it != textureMap_.end()) {
        return it->second.get(); // unique_ptr에서 raw 포인터 반환
    }
    SDL_Log("TextureManager::getTexture - Texture not found: %s", filePath.string().c_str());
    return nullptr;
}