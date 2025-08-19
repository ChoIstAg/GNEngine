#include "LogoScene.h"
#include "RootPath.h"

#include <iostream>
#include <string>

/* --- Include required managers --- */
#include "engine/manager/EntityManager.h"
#include "engine/manager/EventManager.h"
#include "engine/manager/TextureManager.h"
#include "engine/manager/RenderManager.h"
#include "engine/manager/SoundManager.h"
#include "engine/manager/AnimationManager.h"

/* --- Include required components --- */
#include "engine/component/TransformComponent.h"
#include "engine/component/RenderComponent.h"
#include "engine/component/CameraComponent.h"
#include "engine/component/SoundComponent.h"

/* --- Include required prefabs --- */
#include "engine/prefab/PlayerPrefab.h"

LogoScene::LogoScene(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager, AnimationManager& animationManager, EntityManager& entityManager)
    : eventManager_(eventManager),
      renderManager_(renderManager),
      textureManager_(textureManager),
      soundManager_(soundManager),
      animationManager_(animationManager),
      entityManager_(entityManager)
{
    
    logoEntity_ = entityManager_.createEntity();
    entityManager_.addComponent<TransformComponent>(logoEntity_);
    entityManager_.addComponent<RenderComponent>(logoEntity_);

    


    std::cerr << "LogoScene is successfully initialized.\n";
}