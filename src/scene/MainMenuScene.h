#pragma once
#include "Scene.h"
#include <iostream>
#include "engine/ecs/EntityManager.h"

class MainMenuScene : public Scene {
public:
    MainMenuScene(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager, EntityManager& entityManager)
        : Scene(eventManager, renderManager, textureManager, soundManager, entityManager) {}
    ~MainMenuScene() override = default;

    void onEnter() override;
    void onExit() override;
    void handleEvent(const SDL_Event& e) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;
};