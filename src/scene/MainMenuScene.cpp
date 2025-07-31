#include "MainMenuScene.h"
#include <iostream>

void MainMenuScene::onEnter(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager) {
    std::cout << "MainMenuScene: Entering..." << std::endl;
    // 여기에 씬 진입 시 필요한 초기화 로직 (예: UI 요소 로드, 배경 음악 재생)을 추가합니다.
}

void MainMenuScene::onExit() {
    std::cout << "MainMenuScene: Exiting..." << std::endl;
    // 여기에 씬 종료 시 필요한 정리 로직 (예: UI 요소 언로드, 배경 음악 중지)을 추가합니다.
}

void MainMenuScene::handleEvent(const SDL_Event& e) {
    // std::cout << "MainMenuScene: Handling event..." << std::endl;
    // 여기에 씬의 이벤트 처리 로직을 추가합니다.
}

void MainMenuScene::update(float deltaTime) {
    // std::cout << "MainMenuScene: Updating..." << std::endl;
    // 여기에 씬의 업데이트 로직을 추가합니다.
}

void MainMenuScene::render(SDL_Renderer* renderer) {
    // std::cout << "MainMenuScene: Rendering..." << std::endl;
    // 여기에 씬의 렌더링 로직을 추가합니다.
    // 예: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // 파란색 배경
    // SDL_RenderClear(renderer);
}