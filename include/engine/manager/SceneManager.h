#pragma once
#include "../../GNEngine_API.h"

#include <memory>
#include <unordered_map>
#include <string>
#include "../core/Scene.h"

// 순환참조 방지용 전방선언
class EventManager;
class RenderManager;
class TextureManager;
class SoundManager;
class EntityManager;

class GNEngine_API SceneManager {
private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes_;
    Scene* currentScene_ = nullptr;

    // 매니저들에 대한 참조 (Application에서 주입받음)
    EventManager& eventManager_;
    RenderManager& renderManager_;
    TextureManager& textureManager_;
    SoundManager& soundManager_;
    EntityManager& entityManager_;

public:
    SceneManager(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager, EntityManager& entityManager);

    void addScene(const std::string& name, std::unique_ptr<Scene> scene);
    void changeScene(const std::string& name);

    void handleEvent(const Event& event);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
};