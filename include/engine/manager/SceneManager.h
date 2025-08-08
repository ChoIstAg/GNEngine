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
    EventManager* eventManager_ = nullptr;
    RenderManager* renderManager_ = nullptr;
    TextureManager* textureManager_ = nullptr;
    SoundManager* soundManager_ = nullptr;
    EntityManager* entityManager_ = nullptr;

public:
    // 생성자: 필요한 매니저들을 주입받아 초기화
    SceneManager(EventManager* eventManager, RenderManager* renderManager, TextureManager* textureManager, SoundManager* soundManager, EntityManager* entityManager);

    // 씬 추가
    void addScene(const std::string& name, std::unique_ptr<Scene> scene);
    // 씬 변경
    void changeScene(const std::string& name);

    // 현재 활성화된 씬의 생명주기 함수 호출 위임
    void handleEvent(const SDL_Event& event);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
};