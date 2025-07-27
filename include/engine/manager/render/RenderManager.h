#pragma once

#include <SDL3/SDL.h>
#include "engine/resource/texture/Texture.h"

class RenderManager {
private:
    SDL_Renderer* renderer_;
    SDL_Window* window_;

public:
    RenderManager(SDL_Renderer* renderer, SDL_Window* window);
    ~RenderManager();
    //bool init(); // 초기화 함수는 필요 없음.

    void clear();
    void present();

    /* 텍스처를 화면에 그리는 함수 */
    void renderTexture(Texture* texture, float x, float y, float w = 0, float h = 0);
};