#include "RenderComponent.h"

RenderComponent::RenderComponent(RenderManager& renderManager, Texture& texture)
    : renderManager_(renderManager), texture_(texture), transform_(nullptr) {}

void RenderComponent::render() {
    if (!transform_) {
        // For simplicity, this component assumes the parent Object has a TransformComponent.
        // A more robust implementation would handle this case more gracefully.
        return;
    }
    renderManager_.renderTexture(&texture_, transform_->positionX, transform_->positionY);
}
