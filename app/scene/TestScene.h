#pragma once
#include "engine/core/Scene.h"
#include "engine/core/Entity.h"
#include "engine/core/EventInterface.h"
#include "engine/manager/EventManager.h"
#include "engine/manager/TextureManager.h"
#include "engine/manager/RenderManager.h"
#include "engine/manager/SoundManager.h"
#include "engine/manager/AnimationManager.h"
#include <iostream>
#include <memory>

#include "engine/component/SoundComponent.h"

class TestScene : public Scene {
public:
    TestScene(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager, AnimationManager& animationManager, EntityManager& entityManager);
    ~TestScene() override = default;

    void onEnter() override;
    void onExit() override;
    void handleEvent(const Event& event) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    SoundManager& soundManager_;
    AnimationManager& animationManager_;
    RenderManager& renderManager_;
    
    EntityId cameraEntityId_;
};