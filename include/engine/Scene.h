#pragma once
#include <SDL3/SDL.h>

class Scene {
public:
    virtual ~Scene() = default;
    virtual void onEnter() = 0;      // ??ì§„ì… ???¸ì¶œ
    virtual void onExit() = 0;       // ??ì¢…ë£Œ ???¸ì¶œ
    virtual void handleEvent(const SDL_Event& e) = 0; // ?…ë ¥ ì²˜ë¦¬
    virtual void update(float deltaTime) = 0;         // ?¼ë¦¬ ?…ë°?´íŠ¸
    virtual void render(SDL_Renderer* renderer) = 0;  // ?Œë”ë§?
};
