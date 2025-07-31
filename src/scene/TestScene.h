#pragma once
#include "Scene.h"
#include "engine/_legacy/TestObject.h"
#include <iostream>
#include <memory>

class TestScene : public Scene {
public:
    TestScene() = default;
    ~TestScene() override = default;

    void onEnter(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager) override;
    void onExit() override;
    void handleEvent(const SDL_Event& e) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    std::unique_ptr<TestObject> testObject_;
};