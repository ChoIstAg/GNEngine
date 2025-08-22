#include "LogoScene.h"
#include "RootPath.h"

#include <iostream>
#include <string>
#include <filesystem>

#include "GNEngine/core/ComponentArray.h"

/* --- Include required managers --- */
#include "GNEngine/manager/EntityManager.h"
#include "GNEngine/manager/EventManager.h"
#include "GNEngine/manager/RenderManager.h"
#include "GNEngine/manager/SoundManager.h"
#include "GNEngine/manager/TextureManager.h"
#include "GNEngine/manager/AnimationManager.h"
#include "GNEngine/manager/SceneManager.h"
#include "GNEngine/manager/FadeManager.h"

/* --- Include required components --- */
#include "GNEngine/component/TransformComponent.h"
#include "GNEngine/component/RenderComponent.h"
#include "GNEngine/component/CameraComponent.h"
#include "GNEngine/component/SoundComponent.h"
#include "GNEngine/component/FadeComponent.h"

/* --- Include required prefabs --- */
#include "GNEngine/prefab/PlayerPrefab.h"

//TODO [5] - 로고를 여러개 넣을 수 있게 하기. 키 입력시 다음 로고로 넘어가고 마지막 로고시 Fade Out과 함께 장면 전환.

LogoScene::LogoScene(EntityManager& entityManager,
                    SceneManager& sceneManager,
                    EventManager& eventManager,
                    RenderManager& renderManager,
                    SoundManager& soundManager,
                    TextureManager& textureManager,
                    AnimationManager& animationManager,
                    FadeManager& fadeManager)
    : entityManager_(entityManager),
    sceneManager_(sceneManager), 
    eventManager_(eventManager),
    renderManager_(renderManager),
    soundManager_(soundManager),
    textureManager_(textureManager),
    animationManager_(animationManager),
    fadeManager_(fadeManager)
{
    renderManager_.setBackgroundColor({255, 255, 255, 255}); // White
}

bool LogoScene::loadScene() {
    logoEntity_ = entityManager_.createEntity();
    entityManager_.addComponent<TransformComponent>(logoEntity_);

    std::filesystem::path logoPath = static_cast<std::filesystem::path>(IMAGE_ASSET_ROOT_PATH) / "logo/" / "logo_no_background.png";
    Texture* logoIMG = textureManager_.getTexture(logoPath);
    if(logoIMG == nullptr) {
        std::cerr << "[ERROR] LogoScene - can't load logoIMG \n";
    }
    entityManager_.addComponent<RenderComponent>(logoEntity_, logoIMG, RenderLayer::UI);

    /* Skip to input key */
    auto skipLogoScene = [&](const KeyReleasedEvent& event) {
        if(event.keyCode == SDL_SCANCODE_SPACE) {
            screenTime_ = 0.3f;
            fadeInTime_ = 0.1f; // 사실상 의미 없긴 함. 나중에 Fade 로직 변경시 의미 있을것.
            fadeOutTime_ = 0.2f;
        }
    };
    eventManager_.subscribe<KeyReleasedEvent>(skipLogoScene);

    std::cerr << "LogoScene is successfully loaded.\n";
    isLoaded_ = true;
    return true;
}

void LogoScene::onEnter() {
    if (!isLoaded_) {
        loadScene();
    }

    std::cerr << "LogoScene::onEnter()\n";
    currentState_ = LogoSceneState::FADING_IN;
    fadeManager_.startFadeIn(0.3f, {0, 0, 0, 255}, 
        [this]() { 
            currentState_ = LogoSceneState::DISPLAYING;
            sceneTimer_ = 0.0f; // init
        } );
}

void LogoScene::onExit() {
    std::cerr << "LogoScene::onExit()\n";
    entityManager_.destroyEntity(logoEntity_); // 씬 종료 시 로고 엔티티 파괴
}

void LogoScene::update(float deltaTime) {
    if (currentState_ == LogoSceneState::DISPLAYING) {
        sceneTimer_ += deltaTime;
        if (sceneTimer_ >= screenTime_) {
            currentState_ = LogoSceneState::FADING_OUT;
            fadeManager_.startFadeOut(2.0f, {0, 0, 0, 255}, 
                [this]() {
                    sceneManager_.loadScene("TestScene");
                    sceneManager_.changeScene("TestScene");
                } );
        }
    }
}

void LogoScene::render(SDL_Renderer* rawRenderer) {
    SDL_SetRenderDrawColor(rawRenderer, 255, 255, 255, 255); // 흰색 배경
    SDL_RenderClear(rawRenderer);
}

void LogoScene::handleEvent(const Event& event) {
    // 로고 씬에서는 현재 이벤트를 특별히 처리할 필요 없음
}

