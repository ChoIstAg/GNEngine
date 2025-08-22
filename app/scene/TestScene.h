#pragma once
#include "GNEngine_API.h"
#include "GNEngine/core/Scene.h"

#include <vector>

// 필요한 Manager들의 전방 선언
class EventManager;
class RenderManager;
class TextureManager;
class SoundManager;
class AnimationManager;
class EntityManager;

#include "GNEngine/core/Entity.h"

class TestScene : public Scene {
public:
    TestScene(EventManager& eventManager, 
            RenderManager& renderManager, 
            TextureManager& textureManager, 
            SoundManager& soundManager, 
            AnimationManager& animationManager,
            EntityManager& entityManager);
    ~TestScene() override = default;

    void onEnter() override;
    void onExit() override;
    bool loadScene() override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* rawRenderer) override;
    void handleEvent(const Event& event) override;

private:
    EntityManager& entityManager_;
    EventManager& eventManager_;
    RenderManager& renderManager_;
    TextureManager& textureManager_;
    SoundManager& soundManager_;
    AnimationManager& animationManager_;

    EntityID playerEntity_;
    EntityID cameraEntity_;
    std::vector<EntityID> entityIDs_;
};

