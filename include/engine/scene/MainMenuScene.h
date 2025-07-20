#pragma once
#include "Scene.h"
#include <SDL3/SDL.h>
#include <iostream>

class MainMenuScene : public Scene {
public:
    void onEnter() override {
        std::cout << "MainMenuScene 진입" << std::endl;
        // 초기화 코드
    }
    void onExit() override {
        std::cout << "MainMenuScene 종료" << std::endl;
        // 정리 코드
    }
    void handleEvent(const SDL_Event& e) override {
        // 입력 처리
        if (e.type == SDL_EVENT_KEY_DOWN) {
            // 엔터키 누르면 게임 시작
        }
    }
    void update(float deltaTime) override {
        // 논리 업데이트
    }
    void render(SDL_Renderer* renderer) override {
        // 화면 그리기
    }
};