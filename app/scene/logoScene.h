#pragma once
#include "engine/core/Scene.h"

// 필요한 Manager들의 전방 선언
class EventManager;
class RenderManager;
class TextureManager;
class SoundManager;
class AnimationManager;
class EntityManager;

#include "engine/component/TransformComponent.h"
#include "engine/component/TextComponent.h"
#include "engine/component/RenderComponent.h"

class LogoScene : Scene {
public: 
    LogoScene(EventManager& eventManager, 
            RenderManager& renderManager, 
            TextureManager& textureManager, 
            SoundManager& soundManager, 
            AnimationManager& animationManager, 
            EntityManager& entityManager);
    ~LogoScene() override = default;

    void onEnter() override;
    void onExit() override;

private:
    EntityId logoEntity_;

    EventManager& eventManager_;
    RenderManager& renderManager_;
    TextureManager& textureManager_;
    SoundManager& soundManager_;
    AnimationManager& animationManager_;
    EntityManager& entityManager_;
};