#include "engine/manager/RenderManager.h"
#include <iostream>

/* 
 * @brief RenderManager의 생성자.
 * @param renderer SDL_CreateRenderer로 초기화가 끝난 SDL_Renderer 객체
 * @param window SDL_CreateWindow로 초기화가 끝난 SDL_Window 객체
 * @return 초기화 성공 여부 (true: 성공, false: 실패)
 */
RenderManager::RenderManager(SDL_Renderer* renderer, SDL_Window* window)
    : renderer_(renderer), window_(window) {
    if (!renderer_ || !window_) {
        SDL_Log("RenderManager::init - Renderer or Window is null: %s", SDL_GetError());
    }
}

// bool RenderManager::init() {
//     return true;
// }

RenderManager::~RenderManager() {
    // Renderer와 Window는 Application 클래스에서 소유하고 파괴하므로, 여기서는 파괴하지 않음.
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

void RenderManager::setCameraPosition(float x, float y) {
    cameraX_ = x;
    cameraY_ = y;
}

/* 텍스처 렌더링. 
 * @brief 텍스처를 지정된 위치에 렌더링함.
 * @param texture 렌더링할 텍스처 객체
 * @param w, h 텍스처의 너비와 높이. 0이면 텍스처의 원본 크기를 사용함.
 * @param flip SDL_FlipMode 플래그로 좌우/상하 반전 여부를 지정함. (SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL)
*/
void RenderManager::renderTexture(Texture* texture, float x, float y, float w, float h, SDL_FlipMode flip) {
    // 카메라 위치를 적용하여 화면 좌표 계산
    float screenX = (x - cameraX_) * zoomLevel_ + (getWindowWidth() / 2.0f);
    float screenY = (y - cameraY_) * zoomLevel_ + (getWindowHeight() / 2.0f);

    SDL_FRect dstRect = {screenX - (w * zoomLevel_) / 2.0f, screenY - (h * zoomLevel_) / 2.0f, w * zoomLevel_, h * zoomLevel_};
    if (!SDL_RenderTextureRotated(renderer_, texture->sdlTexture_, nullptr, &dstRect, 0.0, nullptr, flip)) {
        SDL_Log("RenderManager::renderTexture - Failed to render texture: %s", SDL_GetError());
    }
}

void RenderManager::renderTexture(Texture* texture, float x, float y, const SDL_Rect* srcRect, float w, float h, SDL_FlipMode flip) {
    if (!renderer_) {
        SDL_Log("RenderManager::renderTexture - Renderer is null. : %s", SDL_GetError());
        return;
    }
    if (!texture) {
        SDL_Log("RenderManager::renderTexture - Texture object is null. : %s", SDL_GetError());
        return;
    }
    if (!texture->sdlTexture_) {
        SDL_Log("RenderManager::renderTexture - SDL_Texture* inside Texture object is null. : %s", SDL_GetError());
        return;
    }

    SDL_FRect dstRect;
    SDL_FRect srcFRect;

    if (srcRect) {
        srcFRect.x = static_cast<float>(srcRect->x);
        srcFRect.y = static_cast<float>(srcRect->y);
        srcFRect.w = static_cast<float>(srcRect->w);
        srcFRect.h = static_cast<float>(srcRect->h);
    }
    
    /* 너비와 높이가 0이면 srcRect 또는 텍스처의 원본 크기를 사용 */
    if (w == 0 || h == 0) {
        if (srcRect) {
            dstRect.w = static_cast<float>(srcRect->w) * zoomLevel_;
            dstRect.h = static_cast<float>(srcRect->h) * zoomLevel_;
        } else {
            dstRect.w = static_cast<float>(texture->width_) * zoomLevel_;
            dstRect.h = static_cast<float>(texture->height_) * zoomLevel_;
        }
    } else {
        dstRect.w = w * zoomLevel_;
        dstRect.h = h * zoomLevel_;
    }

    // 카메라 위치를 적용하여 화면 좌표 계산
    float screenX = (x - cameraX_) * zoomLevel_ + (getWindowWidth() / 2.0f);
    float screenY = (y - cameraY_) * zoomLevel_ + (getWindowHeight() / 2.0f);

    dstRect.x = screenX - dstRect.w / 2.0f; // Adjust x to center
    dstRect.y = screenY - dstRect.h / 2.0f; // Adjust y to center

    /* 텍스처 렌더링 */
    if (!SDL_RenderTextureRotated(renderer_, texture->sdlTexture_, srcRect ? &srcFRect : nullptr, &dstRect, 0.0, nullptr, flip)) {
        SDL_Log("RenderManager::renderTexture - Failed to render texture: %s", SDL_GetError());
    }
}

void RenderManager::setViewport(int x, int y, int w, int h) {
    if (!renderer_) {
        SDL_Log("RenderManager::setViewport - Renderer is null.");
        return;
    }
    SDL_Rect viewport = {x, y, w, h};
    if (!SDL_SetRenderViewport(renderer_, &viewport)) {
        SDL_Log("RenderManager::setViewport - Failed to set viewport: %s", SDL_GetError());
    }
}

int RenderManager::getWindowWidth() const {
    int w, h;
    SDL_GetWindowSize(window_, &w, &h);
    return w;
}

int RenderManager::getWindowHeight() const {
    int w, h;
    SDL_GetWindowSize(window_, &w, &h);
    return h;
}   