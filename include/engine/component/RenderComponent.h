#pragma once

#include "engine/component/Component.h"
#include "engine/resource/texture/Texture.h"
#include "engine/manager/render/RenderManager.h"
#include "engine/component/TransformComponent.h"

class RenderComponent : public Component {
public:
    RenderComponent(RenderManager& renderManager, Texture& texture);

    void render();

private:
    RenderManager& renderManager_;
    Texture& texture_;
    TransformComponent* transform_;
};
