#pragma once
#include <SDL3/SDL.h>

class Scene {
public:
    virtual ~Scene() = default;
    virtual void onEnter() = 0;      // 씬 진입 시 호출
    virtual void onExit() = 0;       // 씬 종료 시 호출
    virtual void handleEvent(const SDL_Event& e) = 0; // 입력 처리
    virtual void update(float deltaTime) = 0;         // 논리 업데이트
    virtual void render(SDL_Renderer* renderer) = 0;  // 렌더링
};