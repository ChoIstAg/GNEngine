#pragma once
#include "../GNEngine_API.h"

#include "GNEngine/manager/EntityManager.h"
#include "GNEngine/manager/TextManager.h"
#include "GNEngine/component/TextComponent.h"
#include "GNEngine/component/RenderComponent.h"

#include <SDL3/SDL.h>

/* 
 *              
*/
class GNEngine_API TextSystem {
public:
    TextSystem(EntityManager& entityManager, TextManager& textManager, SDL_Renderer* renderer);
    ~TextSystem();

    void update(EntityManager& entityManager, float deltaTime);

private:
    EntityManager& entityManager_;
    TextManager& textManager_;
    SDL_Renderer* renderer_;
};