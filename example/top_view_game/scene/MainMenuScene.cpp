#include "MainMenuScene.h"
#include <iostream>

MainMenuScene::MainMenuScene() {

}

bool MainMenuScene::loadScene() {
    
    isLoaded_ = true;
    return true;
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

