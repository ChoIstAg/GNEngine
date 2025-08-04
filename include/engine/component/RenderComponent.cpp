#include "RenderComponent.h"
#include <iostream>

RenderComponent::RenderComponent(Texture* texture, bool hasAnimation)
    : texture_(texture), hasAnimation_(hasAnimation) {}