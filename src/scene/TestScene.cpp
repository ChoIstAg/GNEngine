#include "TestScene.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <filesystem>
#include "engine/ecs/EntityManager.h"

TestScene::TestScene(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager, AnimationManager& animationManager, EntityManager& entityManager)
    : Scene(eventManager, renderManager, textureManager, soundManager, entityManager),
      soundManager_(soundManager),
      animationManager_(animationManager)
{
    // BGM 사운드 로드 및 재생
    std::filesystem::path bgmPath = std::filesystem::path(SOUND_ASSET_ROOT_PATH) / "TestMp3.mp3";
    auto bgmSound = soundManager_.getSound(bgmPath);
    if (bgmSound) {
        bgm_ = std::make_unique<SoundComponent>(soundManager_, bgmSound);
        bgm_->setVolume(1.0f);
        bgm_->setLoop(true);
        bgm_->play();
    }
}

void TestScene::onEnter() {
    std::cout << "TestScene: Entering..." << std::endl;

    // example_png.png 로드
    std::filesystem::path imagePath = std::filesystem::path(IMAGE_ASSET_ROOT_PATH) / "example_png.png";
    if (!textureManager_.loadTexture(imagePath)) {
        std::cerr << "Error: Failed to load example_png.png" << std::endl;
        return;
    }
    Texture* exampleTexture = textureManager_.getTexture(imagePath);

    if (!exampleTexture) {
        std::cerr << "Error: example_png.png texture is null." << std::endl;
        return;
    }

    // 1000개의 엔티티 생성 및 컴포넌트 추가
    for (int i = 0; i < 1000; ++i) {
        EntityId entity = entityManager_.createEntity();

        // 무작위 위치 설정 (화면 크기 내에서)
        float randomX = static_cast<float>(rand() % 1280); // 예시 화면 너비
        float randomY = static_cast<float>(rand() % 720);  // 예시 화면 높이

        entityManager_.addComponent<TransformComponent>(entity, randomX, randomY);
        entityManager_.addComponent<RenderComponent>(entity, exampleTexture, false);
    }

    // 뷰포트 설정: 화면의 왼쪽 절반 (x=0, y=0, width=640, height=720)
    renderManager_.setViewport(0, 0, 640, 720);
}

void TestScene::onExit() {
    std::cout << "TestScene: Exiting..." << std::endl;
    soundManager_.stopAllSounds();
    
}

void TestScene::handleEvent(const SDL_Event& event) {
    // std::cout << "TestScene: Handling event..." << std::endl;
}

void TestScene::update(float deltaTime) {
    // std::cout << "TestScene: Updating..." << std::endl;
    
}

void TestScene::render(SDL_Renderer* renderer) {
    // std::cout << "TestScene: Rendering..." << std::endl;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 검은색 배경
    SDL_RenderClear(renderer);

    
}