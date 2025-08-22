#pragma once
#include "../../GNEngine_API.h"

#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>

#include "../core/Scene.h"
#include "../core/EventInterface.h"

class GNEngine_API SceneManager {
private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes_;
    Scene* currentScene_ = nullptr;

public:
    SceneManager();

    template<typename T>
    void addScene(const std::string& name, std::unique_ptr<T> scene) {
        static_assert(std::is_base_of<Scene, T>::value, "T must inherit from Scene");
        scenes_[name] = std::move(scene);
    }
    void changeScene(const std::string& name);
    bool loadScene(const std::string& name) {
        auto it = scenes_.find(name);
        if (it != scenes_.end() && it->second.get() != nullptr) {
            return scenes_[name]->loadScene();
        } else {
            std::cerr << "[ERROR] SceneManager::loadScene - '" << name << "' scene is not exist. \n";
            return false;
        }
    }

    void handleEvent(const Event& event);
    void update(float deltaTime);
};
