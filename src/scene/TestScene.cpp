#include "TestScene.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <filesystem>

void TestScene::onEnter() {
    std::cout << "TestScene: Entering..." << std::endl;
    testObject_ = std::make_unique<TestObject>(eventManager_, textureManager_, renderManager_, soundManager_);

    // // BGM을 2D 스테레오 사운드로 재생
    // std::filesystem::path bgmPath = std::filesystem::path(SOUND_ASSET_ROOT_PATH) / "TestMp3.mp3";
    // bgm_ = std::make_unique<SoundComponent>(soundManager_, bgmPath, false, false); // spatialized, attenuation 모두 false
    // bgm_->play(SoundPriority::CRITICAL, 0.5f, 1.0f, true); // 볼륨 0.5, 반복 재생
}

void TestScene::onExit() {
    std::cout << "TestScene: Exiting..." << std::endl;
    soundManager_.stopAllSounds();
    testObject_.reset();
}

void TestScene::handleEvent(const SDL_Event& event) {
    // std::cout << "TestScene: Handling event..." << std::endl;
}

void TestScene::update(float deltaTime) {
    // std::cout << "TestScene: Updating..." << std::endl;
    if (testObject_) {
        testObject_->update(deltaTime);
    }
}

void TestScene::render(SDL_Renderer* renderer) {
    // std::cout << "TestScene: Rendering..." << std::endl;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 검은색 배경
    SDL_RenderClear(renderer);

    if (testObject_) {
        testObject_->render();
    }
}