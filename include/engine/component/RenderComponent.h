#pragma once

#include "engine/component/Component.h"
#include "engine/resource/texture/Texture.h"
#include "engine/manager/render/RenderManager.h"
#include "engine/component/TransformComponent.h"
#include "engine/component/AnimationComponent.h"

class RenderComponent : public Component {
public:
    RenderComponent(Texture* texture, bool hasAnimation = false);

    Texture* getTexture() const { return texture_; }
    bool hasAnimation() const { return hasAnimation_; }

    
    

private:
    Texture* texture_;
    bool hasAnimation_ = false;
};
