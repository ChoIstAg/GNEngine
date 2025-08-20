#include "TestScene.h"
#include "RootPath.h"

#include <iostream>
#include <string>
#include <filesystem>

/* --- Managers --- */
#include "engine/manager/RenderManager.h"
#include "engine/manager/TextureManager.h"
#include "engine/manager/SoundManager.h"
#include "engine/manager/AnimationManager.h"
#include "engine/manager/EntityManager.h"
#include "engine/manager/EventManager.h"

/* --- Components --- */
#include "engine/component/TransformComponent.h"
#include "engine/component/RenderComponent.h"
#include "engine/component/CameraComponent.h"
#include "engine/component/SoundComponent.h"
#include "engine/component/InputControlComponent.h"
#include "engine/component/PlayerMovementComponent.h"
#include "engine/component/PlayerAnimationControllerComponent.h"

/* --- Prefabs --- */
#include "engine/prefab/PlayerPrefab.h"

TestScene::TestScene(EventManager& eventManager, 
                   RenderManager& renderManager, 
                   TextureManager& textureManager, 
                   SoundManager& soundManager, 
                   AnimationManager& animationManager,
                   EntityManager& entityManager)
    : eventManager_(eventManager),
      renderManager_(renderManager),
      textureManager_(textureManager),
      soundManager_(soundManager),
      animationManager_(animationManager),
      entityManager_(entityManager)
{
    // Constructor is now empty
}

bool TestScene::loadScene() {
    /* --- Player --- */
    playerEntity_ = PlayerPrefab::create(entityManager_, eventManager_, textureManager_, renderManager_, soundManager_, animationManager_);
    entityIDs_.push_back(playerEntity_);

    /* --- Camera --- */
    cameraEntity_ = entityManager_.createEntity();
    entityManager_.addComponent<CameraComponent>(cameraEntity_, playerEntity_, 10.0f);
    entityIDs_.push_back(cameraEntity_);

    /* --- BGM --- */
    auto bgmEntity = entityManager_.createEntity();
    std::filesystem::path bgmPath = static_cast<std::filesystem::path>(SOUND_ASSET_ROOT_PATH) / "TestMp3.mp3";
    auto bgmSound = soundManager_.getSound(bgmPath);
    if (bgmSound) {
        auto& soundComp = entityManager_.addComponent<SoundComponent>(bgmEntity);
        soundComp.addSound("bgm", bgmSound, true, 0.5f);
        entityIDs_.push_back(bgmEntity);
        soundComp.play("bgm");
    } else {
        std::cerr << "[ERROR] TestScene - Can't load bgm. \n";
    }

    isLoaded_ = true;
    return true;
}

void TestScene::onEnter() {
    if (!isLoaded_) {
        loadScene();
    }
    std::cerr << "TestScene::onEnter()" << std::endl;
}

void TestScene::onExit() {
    for (auto entity : entityIDs_) {
        entityManager_.destroyEntity(entity);
    }
    entityIDs_.clear();
    isLoaded_ = false;
    std::cerr << "TestScene::onExit()" << std::endl;
}

void TestScene::update(float deltaTime) {
    // This scene might not have complex update logic itself
}

void TestScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Dark grey background
}

void TestScene::handleEvent(const Event& event) {
    
}
