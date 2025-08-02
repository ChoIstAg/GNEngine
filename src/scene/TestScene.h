#pragma once
#include "Scene.h"
#include "engine/object/TestObject.h"
#include "engine/component/SoundComponent.h"
#include <iostream>
#include <memory>

class TestScene : public Scene {
public:
    TestScene(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager)
        : Scene(eventManager, renderManager, textureManager, soundManager), soundManager_(soundManager) {}
    ~TestScene() override = default;

    void onEnter() override;
    void onExit() override;
    void handleEvent(const SDL_Event& event) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    SoundManager& soundManager_;
    std::unique_ptr<TestObject> testObject_;
    std::unique_ptr<SoundComponent> bgm_;
};