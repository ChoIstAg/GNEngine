#pragma once
#include "engine/core/Scene.h"


class MainMenuScene : public Scene {
public:
    MainMenuScene();
    ~MainMenuScene() override = default;

    void onEnter() override;
    void onExit() override;
    void handleEvent(const Event& event) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;
};
