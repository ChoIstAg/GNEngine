#pragma once
#include "Scene.h"
#include "engine/object/TestObject.h"
#include <iostream>
#include <memory>

class TestScene : public Scene {
public:
    TestScene(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager)
        : Scene(eventManager, renderManager, textureManager, soundManager) {}
    ~TestScene() override = default;

    void onEnter() override;
    void onExit() override;
    void handleEvent(const SDL_Event& e) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    std::unique_ptr<TestObject> testObject_;
};