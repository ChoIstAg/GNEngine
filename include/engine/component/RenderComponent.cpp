#include "RenderComponent.h"
#include <iostream>

RenderComponent::RenderComponent(RenderManager& renderManager, Texture& texture, TransformComponent& transform)
    : renderManager_(renderManager), texture_(texture), transform_(&transform) {}

void RenderComponent::render() {
    if (!transform_) {
        std::cerr << "transform_ is errornous\n";
        return;
    }
    renderManager_.renderTexture(&texture_, transform_->positionX_, transform_->positionY_);
}
