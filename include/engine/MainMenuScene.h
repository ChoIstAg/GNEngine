#pragma once
#include "Scene.h"
#include <SDL3/SDL.h>
#include <iostream>

class MainMenuScene : public Scene {
public:
    void onEnter() override {
        std::cout << "MainMenuScene 진입" << std::endl;
        // 초기??코드
    }
    void onExit() override {
        std::cout << "MainMenuScene 종료" << std::endl;
        // ?�리 코드
    }
    void handleEvent(const SDL_Event& e) override {
        // ?�력 처리
        if (e.type == SDL_EVENT_KEY_DOWN) {
            // ?�터???�르�?게임 ?�작
        }
    }
    void update(float deltaTime) override {
        // ?�리 ?�데?�트
    }
    void render(SDL_Renderer* renderer) override {
        // ?�면 그리�?
    }
};
