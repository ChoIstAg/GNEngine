#pragma once
#include <SDL3/SDL.h>

class Scene {
public:
    virtual ~Scene() = default;
    virtual void onEnter() = 0;      // ??진입 ???�출
    virtual void onExit() = 0;       // ??종료 ???�출
    virtual void handleEvent(const SDL_Event& e) = 0; // ?�력 처리
    virtual void update(float deltaTime) = 0;         // ?�리 ?�데?�트
    virtual void render(SDL_Renderer* renderer) = 0;  // ?�더�?
};
