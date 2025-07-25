#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Scene.h"

class SceneManager {
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
    Scene* current = nullptr;
public:
    void addScene(const std::string& name, std::unique_ptr<Scene> scene);
    void changeScene(const std::string& name);
    void handleEvent(const SDL_Event& e);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
};