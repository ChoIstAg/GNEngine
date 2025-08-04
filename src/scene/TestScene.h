#pragma once
#include "Scene.h"
#include "engine/object/TestObject.h" // TestObject is now a helper class
#include "engine/component/SoundComponent.h"
#include <iostream>
#include <memory>

class TestScene : public Scene {
public:
    TestScene(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager, AnimationManager& animationManager, EntityManager& entityManager);
    ~TestScene() override = default;

    void onEnter() override;
    void onExit() override;
    void handleEvent(const SDL_Event& event) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    SoundManager& soundManager_;
    AnimationManager& animationManager_;
    std::unique_ptr<SoundComponent> bgm_;
};