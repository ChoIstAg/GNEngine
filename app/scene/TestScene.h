#pragma once
#include "engine/core/Scene.h"
#include "engine/core/Entity.h"

// 필요한 Manager들의 전방 선언
class EventManager;
class RenderManager;
class TextureManager;
class SoundManager;
class AnimationManager;
class EntityManager;

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
    void handleEvent(const Event& event) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    // 이 씬에서 사용하는 Manager들에 대한 참조
    EventManager& eventManager_;
    RenderManager& renderManager_;
    TextureManager& textureManager_;
    SoundManager& soundManager_;
    AnimationManager& animationManager_;
    EntityManager& entityManager_;
    
    EntityId cameraEntityId_;
};
