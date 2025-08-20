#include "LogoScene.h"
#include "RootPath.h"

#include <iostream>
#include <string>
#include <filesystem>

#include "engine/core/ComponentArray.h"

/* --- Include required managers --- */
#include "engine/manager/EntityManager.h"
#include "engine/manager/EventManager.h"
#include "engine/manager/RenderManager.h"
#include "engine/manager/SoundManager.h"
#include "engine/manager/TextureManager.h"
#include "engine/manager/AnimationManager.h"
#include "engine/manager/SceneManager.h"
#include "engine/manager/FadeManager.h"

/* --- Include required components --- */
#include "engine/component/TransformComponent.h"
#include "engine/component/RenderComponent.h"
#include "engine/component/CameraComponent.h"
#include "engine/component/SoundComponent.h"
#include "engine/component/FadeComponent.h"

/* --- Include required prefabs --- */
#include "engine/prefab/PlayerPrefab.h"

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
    // Constructor is now empty
}

bool LogoScene::loadScene()
{
    logoEntity_ = entityManager_.createEntity();
    entityManager_.addComponent<TransformComponent>(logoEntity_);

    std::filesystem::path logoPath = static_cast<std::filesystem::path>(IMAGE_ASSET_ROOT_PATH) / "logo/" / "logo_no_background.png";
    Texture* logoIMG = textureManager_.getTexture(logoPath);
    if(logoIMG == nullptr) { 
        std::cerr << "[ERROR] LogoScene - can't load logoIMG \n";
    }
    entityManager_.addComponent<RenderComponent>(logoEntity_, logoIMG);

    // 로고를 화면 중앙에 배치
    TransformComponent transform = entityManager_.getComponent<TransformComponent>(logoEntity_).value();
    transform.positionX_ = renderManager_.getWindowWidth() / 2.0f;
    transform.positionY_ = renderManager_.getWindowHeight() / 2.0f;
    
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
    fadeManager_.startFadeIn(2.0f, {0, 0, 0, 255}, 
        [this]() { 
            currentState_ = LogoSceneState::DISPLAYING;
            sceneTimer_ = 0.0f;
        } );
}

void LogoScene::onExit() {
    std::cerr << "LogoScene::onExit()\n";
    entityManager_.destroyEntity(logoEntity_); // 씬 종료 시 로고 엔티티 파괴
}

void LogoScene::update(float deltaTime) {
    if (currentState_ == LogoSceneState::DISPLAYING)
    {
        sceneTimer_ += deltaTime;
        if (sceneTimer_ >= 3.0f)
        {
            currentState_ = LogoSceneState::FADING_OUT;
            fadeManager_.startFadeOut(2.0f, {0, 0, 0, 255}, 
                [this]() {
                    sceneManager_.changeScene("TestScene");
                });
        }
    }
}

void LogoScene::render(SDL_Renderer* rawRenderer) {
    SDL_SetRenderDrawColor(renderManager_.getRenderer(), 255, 255, 255, 255); // 흰색 배경
}

void LogoScene::handleEvent(const Event& event) {
    // 로고 씬에서는 현재 이벤트를 특별히 처리할 필요 없음
}
