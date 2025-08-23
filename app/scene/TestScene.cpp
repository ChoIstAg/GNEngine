#include "TestScene.h"
#include "RootPath.h"

#include <iostream>
#include <string>
#include <filesystem>

/* --- Managers --- */
#include "GNEngine/manager/EntityManager.h"
#include "GNEngine/manager/EventManager.h"
#include "GNEngine/manager/RenderManager.h"
#include "GNEngine/manager/TextureManager.h"
#include "GNEngine/manager/SoundManager.h"
#include "GNEngine/manager/AnimationManager.h"
#include "GNEngine/manager/TextManager.h"

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

TestScene::TestScene(EntityManager& entityManager,
                    EventManager& eventManager,
                    RenderManager& renderManager,
                    SoundManager& soundManager,
                    TextureManager& textureManager,
                    TextManager& textManager,
                    AnimationManager& animationManager)
    : entityManager_(entityManager),
    eventManager_(eventManager),
    renderManager_(renderManager),
    textureManager_(textureManager),
    textManager_(textManager),
    soundManager_(soundManager),
    animationManager_(animationManager)
{
}

bool TestScene::loadScene() {
    
    /* --- Player --- */
    playerEntity_ = PlayerPrefab::create(entityManager_, eventManager_, textureManager_, renderManager_, soundManager_, animationManager_);
    entityIDs_.push_back(playerEntity_);
    // std::cerr << "[DEBUG] TestScene::loadScene -  Player is successfully loaded.\n";
    
    /* --- Camera --- */
    cameraEntity_ = entityManager_.createEntity();
    entityManager_.addComponent<CameraComponent>(cameraEntity_, playerEntity_, 10.0f);
    entityIDs_.push_back(cameraEntity_);
    // std::cerr << "[DEBUG] TestScene::loadScene - Camera is successfully loaded.\n";
    
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
    // std::cerr << "[DEBUG] TestScene::loadScene - bgm is successfully loaded.\n";
    
    
    auto textEntity = entityManager_.createEntity();
    entityIDs_.push_back(textEntity);
    std::filesystem::path textPath = static_cast<std::filesystem::path>(TEXT_ASSET_ROOT_PATH) / "test.txt";
    
    
    isLoaded_ = true;
    return true;
}

void TestScene::onEnter() {
    if (!isLoaded_) {
        loadScene();
    }
    renderManager_.setBackgroundColor({0, 0, 0, 255}); // black
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



void TestScene::handleEvent(const Event& event) {
    
}

