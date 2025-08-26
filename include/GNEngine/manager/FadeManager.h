#pragma once
#include "../GNEngine_API.h"

#include <SDL3/SDl.h>
#include <functional>

class EntityManager;

/* 
 * ?붾㈃ ?꾩껜??Fade ?④낵瑜??곸슜?쒗궓??
*/
class GNEngine_API FadeManager {
public:
    FadeManager(EntityManager& entityManager);

    /* Fade effects */
    void startFadeIn(float duration, SDL_Color color = { 0, 0, 0, 255 }, std::function<void()> onComplete = nullptr);
    void startFadeOut(float duration, SDL_Color color = { 0, 0, 0, 255 }, std::function<void()> onComplete = nullptr);

private:
    EntityManager& entityManager_;
};


