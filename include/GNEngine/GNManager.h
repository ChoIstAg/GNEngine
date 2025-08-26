#pragma once

#include <memory> // Required for std::unique_ptr
#include <SDL3/SDL.h> // Required for SDL_Renderer* and SDL_Window*

// Include all manager headers
#include "GNEngine/manager/FileManager.h" // For FileManager::getInstance()
#include "GNEngine/manager/EntityManager.h"
#include "GNEngine/manager/EventManager.h"
#include "GNEngine/manager/InputManager.h"
#include "GNEngine/manager/SoundManager.h"
#include "GNEngine/manager/TextureManager.h"
#include "GNEngine/manager/TextManager.h"
#include "GNEngine/manager/AnimationManager.h"
#include "GNEngine/manager/FadeManager.h"
#include "GNEngine/manager/SystemManager.h"
#include "GNEngine/manager/SceneManager.h"
#include "GNEngine/manager/RenderManager.h"

#include "GNEngine_API.h" // Include the API export macro

struct GNEngine_API GNManager {
private:
    // Private constructor to enforce singleton pattern
    GNManager(SDL_Renderer* renderer, SDL_Window* window);

    // Delete copy and move constructors/assignment operators
    GNManager(const GNManager&) = delete;
    GNManager& operator=(const GNManager&) = delete;
    GNManager(GNManager&&) = delete;
    GNManager& operator=(GNManager&&) = delete;

    // Managers
    // The destruction order is the reverse of the declaration order.
    // Declare in the order of creation/dependency.
    // FileManager is a singleton, so it's not owned by GNManager.
    std::unique_ptr<EntityManager> entityManager_;
    std::unique_ptr<EventManager> eventManager_;
    std::unique_ptr<InputManager> inputManager_;
    std::unique_ptr<SoundManager> soundManager_;
    std::unique_ptr<TextureManager> textureManager_;
    std::unique_ptr<TextManager> textManager_;
    std::unique_ptr<AnimationManager> animationManager_;
    std::unique_ptr<FadeManager> fadeManager_;
    std::unique_ptr<SystemManager> systemManager_;
    std::unique_ptr<SceneManager> sceneManager_;
    std::unique_ptr<RenderManager> renderManager_;

public:
    // Static method to get the singleton instance
    static GNManager& getInstance(SDL_Renderer* renderer = nullptr, SDL_Window* window = nullptr);

    // Destructor (unique_ptr handles destruction, but can add custom logic if needed)
    ~GNManager() = default;

    // Getter methods for each manager
    // FileManager is a singleton, so its getter is static.
    static FileManager& getFileManager();
    EntityManager& getEntityManager();
    EventManager& getEventManager();
    InputManager& getInputManager();
    SoundManager& getSoundManager();
    TextureManager& getTextureManager();
    TextManager& getTextManager();
    AnimationManager& getAnimationManager();
    FadeManager& getFadeManager();
    SystemManager& getSystemManager();
    SceneManager& getSceneManager();
    RenderManager& getRenderManager();
};


