#pragma once
#include "../../GNEngine_API.h"

#include "engine/core/Component.h"
#include "engine/resource/Texture.h"
#include "engine/manager/RenderManager.h"
#include "engine/component/TransformComponent.h"
#include "engine/component/AnimationComponent.h"

class GNEngine_API RenderComponent : public Component {
public:
    RenderComponent(Texture* texture, bool hasAnimation = false, SDL_Rect srcRect = {0,0,0,0}, bool flipX = false, bool flipY = false);

    Texture* getTexture() const { return texture_; }
    void setTexture(Texture* texture) { texture_ = texture; }
    bool hasAnimation() const { return hasAnimation_; }
    void setHasAnimation(bool hasAnim) { hasAnimation_ = hasAnim; }
    const SDL_Rect& getSrcRect() const { return srcRect_; }
    void setSrcRect(const SDL_Rect& rect) { srcRect_ = rect; }

    bool getFlipX() const { return flipX_; }
    void setFlipX(bool flip) { flipX_ = flip; }
    bool getFlipY() const { return flipY_; }
    void setFlipY(bool flip) { flipY_ = flip; }

private:
    Texture* texture_;
    bool hasAnimation_ = false;
    SDL_Rect srcRect_;
    bool flipX_ = false;
    bool flipY_ = false;
};
