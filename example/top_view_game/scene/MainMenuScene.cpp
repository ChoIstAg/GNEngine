#include "MainMenuScene.h"
#include <iostream>

MainMenuScene::MainMenuScene() {
    // ?꾩슂??Manager媛 ?덈떎硫??ш린??二쇱엯諛쏆븘 珥덇린?뷀빀?덈떎.
}

bool MainMenuScene::loadScene() {
    // ???ъ뿉 ?꾩슂???뷀떚?? 由ъ냼???깆쓣 濡쒕뱶?⑸땲??
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

