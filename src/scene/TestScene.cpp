#include "TestScene.h"
#include <SDL3/SDL.h>
#include "config.h"
#include <filesystem>
#include <iostream>

#include "engine/manager/EntityManager.h"
#include "engine/component/CameraComponent.h"
#include "engine/component/RenderComponent.h"
#include "engine/component/TransformComponent.h"

#include "engine/prefab/PlayerPrefab.h"

TestScene::TestScene(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager, AnimationManager& animationManager, EntityManager& entityManager)
    : Scene(eventManager, renderManager, textureManager, soundManager, entityManager), 
        renderManager_(renderManager),
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

    EntityId exampleEntityId = entityManager.createEntity();
    entityManager.addComponent<TransformComponent>(exampleEntityId, 100.0f, 100.0f);
    textureManager.loadTexture(std::filesystem::path(IMAGE_ASSET_ROOT_PATH) / "example_png.png");
    entityManager.addComponent<RenderComponent>(exampleEntityId, textureManager.getTexture(std::filesystem::path(IMAGE_ASSET_ROOT_PATH) / "example_png.png"), false);
    

    // PlayerFactory를 사용하여 플레이어 엔티티 생성
    EntityId playerEntityId = PlayerPrefab::create(entityManager, eventManager, textureManager, renderManager, soundManager, animationManager);

    // 카메라 엔티티 생성 및 CameraComponent 추가
    cameraEntityId_ = entityManager.createEntity();
    // PlayerFactory가 생성한 플레이어 엔티티를 카메라의 타겟으로 설정
    entityManager.addComponent<CameraComponent>(cameraEntityId_, 0.0f, 0.0f, 2.0f, playerEntityId);
}

void TestScene::onEnter() {
    std::cout << "TestScene: Entering..." << std::endl;

    renderManager_.setViewport(0, 0, 1280, 720); // 전체 화면 뷰포트 (임시)
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