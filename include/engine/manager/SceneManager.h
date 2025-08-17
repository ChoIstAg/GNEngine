#pragma once
#include "../../GNEngine_API.h"

#include <memory>
#include <unordered_map>
#include <string>
#include "../core/Scene.h"

// EventInterface는 handleEvent 시그니처에 필요함
#include "../core/EventInterface.h"

class GNEngine_API SceneManager {
private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes_;
    Scene* currentScene_ = nullptr;

public:
    SceneManager();

    void addScene(const std::string& name, std::unique_ptr<Scene> scene);
    void changeScene(const std::string& name);

    void handleEvent(const Event& event);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
};
