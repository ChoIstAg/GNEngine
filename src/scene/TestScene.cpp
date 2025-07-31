#include "TestScene.h"
#include <iostream>
#include <SDL3/SDL.h>

void TestScene::onEnter(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager) {
    std::cout << "TestScene: Entering..." << std::endl;
    testObject_ = std::make_unique<TestObject>(eventManager, textureManager, renderManager, soundManager);
}

void TestScene::onExit() {
    std::cout << "TestScene: Exiting..." << std::endl;
    testObject_.reset();
}

void TestScene::handleEvent(const SDL_Event& e) {
    // std::cout << "TestScene: Handling event..." << std::endl;
}

void TestScene::update(float deltaTime) {
    // std::cout << "TestScene: Updating..." << std::endl;
    if (testObject_) {
        testObject_->update();
    }
}

void TestScene::render(SDL_Renderer* renderer) {
    // std::cout << "TestScene: Rendering..." << std::endl;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // 녹색 배경
    SDL_RenderClear(renderer);

    if (testObject_) {
        testObject_->render();
    }
}