#include "RenderManager.h"
#include <iostream>

RenderManager::RenderManager() {
    std::cerr << "RenderManager " << this << " is successfully created" << std::endl;
}

bool RenderManager::init(SDL_Renderer* renderer, SDL_Window* window) {
    if (!renderer || !window) {
        SDL_Log("RenderManager::init - Renderer or Window is null: %s", SDL_GetError());
        return false;
    }
    renderer_ = renderer;
    window_ = window;

    return true;
}

RenderManager::~RenderManager() {
    // Renderer와 Window는 Application 클래스에서 소유하고 파괴하므로, 여기서는 파괴하지 않습니다.
    std::cerr << "RenderManager " << this << " is successfully destroyed" << std::endl;
}

/* 화면 비우기 */
void RenderManager::clear() {
    if (renderer_) {
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255); /* 기본 배경색을 검정으로 설정 */
        SDL_RenderClear(renderer_);
    }
}

/* 화면에 렌더링된 내용을 실제로 표시. */
void RenderManager::present() {
    if (renderer_) {
        SDL_RenderPresent(renderer_);
    }
}

/* 텍스처 렌더링. 
 * @brief 텍스처를 지정된 위치에 렌더링함.
 * @param texture 렌더링할 텍스처 객체
 * @param w, h 텍스처의 너비와 높이. 0이면 텍스처의 원본 크기를 사용함.
*/
void RenderManager::renderTexture(Texture* texture, float x, float y, float w, float h) {
    if (!renderer_) {
        SDL_Log("RenderManager::renderTexture - Renderer is null.");
        return;
    }
    if (!texture) {
        SDL_Log("RenderManager::renderTexture - Texture object is null.");
        return;
    }
    if (!texture->sdlTexture_) {
        SDL_Log("RenderManager::renderTexture - SDL_Texture* inside Texture object is null.");
        return;
    }

    SDL_FRect dstRect;
    
    // 너비와 높이가 지정되지 않았다면 텍스처의 원본 크기를 사용
    if (w == 0 || h == 0) {
        dstRect.w = static_cast<float>(texture->width_);
        dstRect.h = static_cast<float>(texture->height_);
    } else {
        dstRect.w = w;
        dstRect.h = h;
    }

    dstRect.x = x;
    dstRect.y = y;

    /* 텍스처 전체 렌더링 */
    if (SDL_RenderTexture(renderer_, texture->sdlTexture_, nullptr, &dstRect) == 0) {
        SDL_Log("RenderManager::renderTexture - Failed to render texture: %s", SDL_GetError());
    }
}   