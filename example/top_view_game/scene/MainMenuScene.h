#pragma once
#include "GNEngine/core/Scene.h"


class MainMenuScene : public Scene {
public:
    MainMenuScene();
    ~MainMenuScene() override = default;

    void onEnter() override;
    void onExit() override;
    bool loadScene() override;
    void update(float deltaTime) override;
};