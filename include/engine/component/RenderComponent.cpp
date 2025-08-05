#include "RenderComponent.h"
#include <iostream>

RenderComponent::RenderComponent(Texture* texture, bool hasAnimation, SDL_Rect srcRect, bool flipX, bool flipY)
    : texture_(texture), hasAnimation_(hasAnimation), srcRect_(srcRect), flipX_(flipX), flipY_(flipY) {}