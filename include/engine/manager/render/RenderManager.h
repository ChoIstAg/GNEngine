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

    void clear();
    void present();

    SDL_Renderer* getRenderer() const { return renderer_; }
    SDL_Window* getWindow() const { return window_; }
    

    /* 텍스처를 화면에 그리는 함수 */
    void renderTexture(Texture* texture, float x, float y, float w = 0, float h = 0);
    void renderTexture(Texture* texture, float x, float y, const SDL_Rect* srcRect, float w = 0, float h = 0);

    /*
     * @brief 렌더링 뷰포트를 설정함.
     * @param x 뷰포트의 좌상단 X 좌표.
     * @param y 뷰포트의 좌상단 Y 좌표.
     * @param w 뷰포트의 너비.
     * @param h 뷰포트의 높이.
     */
    void setViewport(int x, int y, int w, int h);
};