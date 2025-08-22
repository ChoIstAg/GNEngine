#pragma once
#include "../../GNEngine_API.h"

#include <SDL3/SDL.h>
#include "GNEngine/resource/Texture.h"

class GNEngine_API RenderManager {
private:
    SDL_Renderer* renderer_;
    SDL_Window* window_;

    float cameraX_ = 0.0f;
    float cameraY_ = 0.0f;
    float zoomLevel_ = 1.0f;
    SDL_Color backgroundColor = {0, 0, 0, 255};

public:
    RenderManager(SDL_Renderer* renderer, SDL_Window* window);
    ~RenderManager();
    
    void clear();
    void present();

    void setViewport(SDL_Rect viewport) { SDL_SetRenderViewport(renderer_, &viewport); }
    
    SDL_Renderer* getRenderer() const { return renderer_; }
    SDL_Window* getWindow() const { return window_; }
    int getWindowWidth() const {
        int w, h;
        SDL_GetWindowSize(window_, &w, &h);
        return w;
    }
    int getWindowHeight() const {
        int w, h;
        SDL_GetWindowSize(window_, &w, &h);
        return h;
    }
    
    void setBackgroundColor(SDL_Color color) { backgroundColor = color; }
    SDL_Color getBackgroundColor() { return backgroundColor; }
    
    void setCameraPosition(float x, float y) { cameraX_ = x; cameraY_ = y; }
    float getCameraX() const { return cameraX_; }
    float getCameraY() const { return cameraY_; }
    void setZoomLevel(float zoom) { zoomLevel_ = zoom; }
    float getZoomLevel() const { return zoomLevel_; }
    
    /* 텍스처를 화면에 그리는 함수 */
    void renderTexture(Texture* texture, float x, float y, float w, float h, SDL_FlipMode flip = SDL_FLIP_NONE);
    void renderTexture(Texture* texture, float x, float y, const SDL_Rect* srcRect, float w, float h, SDL_FlipMode flip = SDL_FLIP_NONE);
    void renderTexture(SDL_Texture* texture, float x, float y, const SDL_Rect* srcRect, float w, float h, SDL_FlipMode flip = SDL_FLIP_NONE);


    
};
