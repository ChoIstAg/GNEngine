#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_hints.h>

#include <memory>
#include <chrono>

class Application {
private:
    SDL_Renderer* renderer_;
    SDL_Window* window_;
    
    int windowWidth;
    int windowHeight;

    bool isRunning_ = false;

    std::chrono::high_resolution_clock::time_point lastFrameTime_;

public:
    Application();
    ~Application();
    int init();

    void run();
    void quit();

    SDL_Renderer* getRenderer() const { return renderer_; }
    SDL_Window* getWindow() const { return window_; }
};