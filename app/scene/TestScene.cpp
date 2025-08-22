#include "TestScene.h"
#include "RootPath.h"

#include <iostream>
#include <string>
#include <filesystem>

/* --- Managers --- */
#include "GNEngine/manager/RenderManager.h"
#include "GNEngine/manager/TextureManager.h"
#include "GNEngine/manager/SoundManager.h"
#include "GNEngine/manager/AnimationManager.h"
#include "GNEngine/manager/EntityManager.h"
#include "GNEngine/manager/EventManager.h"

/* --- Components --- */
#include "GNEngine/component/TransformComponent.h"
#include "GNEngine/component/RenderComponent.h"
#include "GNEngine/component/CameraComponent.h"
#include "GNEngine/component/SoundComponent.h"
#include "GNEngine/component/InputControlComponent.h"
#include "GNEngine/component/PlayerMovementComponent.h"
#include "GNEngine/component/PlayerAnimationControllerComponent.h"

/* --- Prefabs --- */
#include "GNEngine/prefab/PlayerPrefab.h"

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
    entityIDs_.push_back(bgmEntity);
    std::filesystem::path bgmPath = static_cast<std::filesystem::path>(SOUND_ASSET_ROOT_PATH) / "TestMp3.mp3";
    auto bgmSound = soundManager_.getSound(bgmPath);
    if (bgmSound) {
        auto& soundComp = entityManager_.addComponent<SoundComponent>(bgmEntity);
        entityManager_.addComponent<TransformComponent>(bgmEntity);
        soundComp.addSound("bgm", bgmSound, true, 0.5f);
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Black
}

void TestScene::handleEvent(const Event& event) {
    
}

