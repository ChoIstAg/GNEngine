#include "GNEngine/GNManager.h"

// Initialize the static instance pointer to nullptr
static GNManager* s_instance = nullptr;

// Private constructor implementation
GNManager::GNManager(SDL_Renderer* renderer, SDL_Window* window)
    : entityManager_(std::make_unique<EntityManager>()),
      eventManager_(std::make_unique<EventManager>()),
      inputManager_(std::make_unique<InputManager>(*eventManager_)),
      soundManager_(std::make_unique<SoundManager>()),
      textureManager_(std::make_unique<TextureManager>(renderer)),
      textManager_(std::make_unique<TextManager>(renderer)),
      animationManager_(std::make_unique<AnimationManager>()),
      fadeManager_(std::make_unique<FadeManager>(*entityManager_)),
      systemManager_(std::make_unique<SystemManager>(*entityManager_)),
      sceneManager_(std::make_unique<SceneManager>()),
      renderManager_(std::make_unique<RenderManager>(renderer, window))
{
    // Any post-construction initialization for managers can go here if needed
    // For example, setting up inter-manager dependencies that require fully constructed managers.
    // This was previously done in Application::init()
}

// Static getInstance method implementation
GNManager& GNManager::getInstance(SDL_Renderer* renderer, SDL_Window* window) {
    if (s_instance == nullptr) {
        // Ensure renderer and window are provided for the first initialization
        if (renderer == nullptr || window == nullptr) {
            // This should ideally be handled by ensuring init() is called first
            // or by passing these parameters from Application::init()
            // For now, log an error or throw an exception if not provided on first call
            SDL_Log("GNManager::getInstance - Renderer or Window is null on first initialization!");
            // Potentially throw an exception or handle error appropriately
        }
        s_instance = new GNManager(renderer, window);
    }
    return *s_instance;
}

// Getter implementations
FileManager& GNManager::getFileManager() { return FileManager::getInstance(); }
EntityManager& GNManager::getEntityManager() { return *entityManager_; }
EventManager& GNManager::getEventManager() { return *eventManager_; }
InputManager& GNManager::getInputManager() { return *inputManager_; }
SoundManager& GNManager::getSoundManager() { return *soundManager_; }
TextureManager& GNManager::getTextureManager() { return *textureManager_; }
TextManager& GNManager::getTextManager() { return *textManager_; }
AnimationManager& GNManager::getAnimationManager() { return *animationManager_; }
FadeManager& GNManager::getFadeManager() { return *fadeManager_; }
SystemManager& GNManager::getSystemManager() { return *systemManager_; }
SceneManager& GNManager::getSceneManager() { return *sceneManager_; }
RenderManager& GNManager::getRenderManager() { return *renderManager_; }


