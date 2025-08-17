#include "MainMenuScene.h"
#include <iostream>

MainMenuScene::MainMenuScene() {
    // 필요한 Manager가 있다면 여기서 주입받아 초기화합니다.
}

void MainMenuScene::onEnter() {
    std::cout << "MainMenuScene: Entering..." << std::endl;
    
}

void MainMenuScene::onExit() {
    std::cout << "MainMenuScene: Exiting..." << std::endl;
    
}

void MainMenuScene::handleEvent(const Event& event) {
    // std::cout << "MainMenuScene: Handling event..." << std::endl;
    
}

void MainMenuScene::update(float deltaTime) {
    // std::cout << "MainMenuScene: Updating..." << std::endl;
    
}

void MainMenuScene::render(SDL_Renderer* renderer) {
    // std::cout << "MainMenuScene: Rendering..." << std::endl;
}
