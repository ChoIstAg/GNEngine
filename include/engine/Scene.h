#pragma once
#include <SDL3/SDL.h>

class Scene {
public:
    virtual ~Scene() = default;
    virtual void onEnter() = 0; 
    virtual void onExit() = 0;
    virtual void handleEvent(const SDL_Event& e) = 0; 
    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
};
