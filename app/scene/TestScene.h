#pragma once
#include "GNEngine_API.h"
#include "GNEngine/core/Scene.h"

#include <vector>

// 필요한 Manager들의 전방 선언
class EntityManager;
class EventManager;
class RenderManager;
class SoundManager;
class TextureManager;
class TextManager;
class AnimationManager;

#include "GNEngine/core/Entity.h"

class TestScene : public Scene {
public:
    TestScene(EntityManager& entityManager,
            EventManager& eventManager, 
            RenderManager& renderManager, 
            SoundManager& soundManager, 
            TextureManager& textureManager,
            TextManager& textManager, 
            AnimationManager& animationManager);
    ~TestScene() override = default;

    void onEnter() override;
    void onExit() override;
    bool loadScene() override;
    void update(float deltaTime) override;

private:
    EntityManager& entityManager_;
    EventManager& eventManager_;
    RenderManager& renderManager_;
    SoundManager& soundManager_;
    TextureManager& textureManager_;
    TextManager& textManager_;
    AnimationManager& animationManager_;

    EntityID playerEntity_;
    EntityID cameraEntity_;
    std::vector<EntityID> entityIDs_;
};

