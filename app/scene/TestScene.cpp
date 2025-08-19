#include "TestScene.h"

#include "RootPath.h"
#include <SDL3/SDL.h>
#include <filesystem>
#include <iostream>

/* --- Include required managers --- */
#include "engine/manager/EntityManager.h"
#include "engine/manager/EventManager.h"
#include "engine/manager/TextureManager.h"
#include "engine/manager/RenderManager.h"
#include "engine/manager/SoundManager.h"
#include "engine/manager/AnimationManager.h"

/* --- Include required components --- */
#include "engine/component/CameraComponent.h"
#include "engine/component/RenderComponent.h"
#include "engine/component/TransformComponent.h"
#include "engine/component/SoundComponent.h"

/* --- Include required prefabs --- */
#include "engine/prefab/PlayerPrefab.h"

TestScene::TestScene(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager, AnimationManager& animationManager, EntityManager& entityManager)
    : eventManager_(eventManager),
      renderManager_(renderManager),
      textureManager_(textureManager),
      soundManager_(soundManager),
      animationManager_(animationManager),
      entityManager_(entityManager)
{
    // BGM 사운드 로드 및 재생
    std::filesystem::path bgmPath = std::filesystem::path(SOUND_ASSET_ROOT_PATH) / "TestMp3.mp3";
    auto bgmSound = soundManager_.getSound(bgmPath);
    if (bgmSound) {
        bgmEntity = entityManager_.createEntity();
        entityManager_.addComponent<TransformComponent>(bgmEntity);
        auto& soundComponentRef = entityManager_.addComponent<SoundComponent>(bgmEntity);
        soundComponent = &soundComponentRef;
        soundComponent->addSound("bgm", bgmSound, true, 0.3f);
        soundComponent->play("bgm");
        std::cerr << "bgm is successfully played.\n";
    } else { std::cerr << "TestScene: Failed to load BGM sound from " << bgmPath << "\n"; }

    // Example 엔티티 생성
    EntityId exampleEntityId = entityManager_.createEntity();
    entityManager_.addComponent<TransformComponent>(exampleEntityId, 0.0f, 0.0f);
    std::filesystem::path examplePngPath = std::filesystem::path(IMAGE_ASSET_ROOT_PATH) / "example_png.png";
    if (textureManager_.loadTexture(examplePngPath)) {
        SDL_Log("TestScene: example_png.png loaded successfully.");
    } else {
        SDL_Log("TestScene: Failed to load example_png.png.");
    }
    auto exampleTexture = textureManager_.getTexture(examplePngPath);
    entityManager_.addComponent<RenderComponent>(exampleEntityId, exampleTexture, false, SDL_Rect{0, 0, exampleTexture->width_, exampleTexture->height_});
    

    // PlayerFactory를 사용하여 플레이어 엔티티 생성
    EntityId playerEntityId = PlayerPrefab::create(entityManager_, eventManager_, textureManager_, renderManager_, soundManager_, animationManager_);

    // 카메라 엔티티 생성 및 CameraComponent 추가
    cameraEntityId_ = entityManager_.createEntity();
    // PlayerFactory가 생성한 플레이어 엔티티를 카메라의 타겟으로 설정
    entityManager_.addComponent<CameraComponent>(cameraEntityId_, 0.0f, 0.0f, 10.0f, playerEntityId);

    std::cerr << "TestScene successfully initialized! \n";
}

void TestScene::onEnter() {
    std::cout << "TestScene: Entering..." << std::endl;
}

void TestScene::onExit() {
    std::cout << "TestScene: Exiting..." << std::endl;
    if (soundComponent) {
        soundComponent->stop("bgm");
    }
}

void TestScene::handleEvent(const Event& event) {
    // std::cout << "TestScene: Handling event..." << std::endl;
}

void TestScene::update(float deltaTime) {
    // std::cout << "TestScene: Updating..." << std::endl;
    
}

void TestScene::render(SDL_Renderer* renderer) {
    // std::cout << "TestScene: Rendering..." << std::endl;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 흰색 배경
    SDL_RenderClear(renderer);
}
