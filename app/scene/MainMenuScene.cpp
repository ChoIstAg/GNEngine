#include "MainMenuScene.h"
#include <iostream>

MainMenuScene::MainMenuScene() {
    // 필요한 Manager가 있다면 여기서 주입받아 초기화합니다.
}

bool MainMenuScene::loadScene() {
    // 이 씬에 필요한 엔티티, 리소스 등을 로드합니다.
    isLoaded_ = true;
}

void MainMenuScene::onEnter() {
    if (!isLoaded_) {
        loadScene();
    }
    std::cout << "MainMenuScene: Entering..." << std::endl;
}

void MainMenuScene::onExit() {
    std::cout << "MainMenuScene: Exiting..." << std::endl;
}

void MainMenuScene::update(float deltaTime) {
    // std::cout << "MainMenuScene: Updating..." << std::endl;
}