#pragma once

#include "engine/event/EventManager.h"
#include "engine/event/EventListnerComponent.h"
#include "engine/event/EventInterface.h"
#include "engine/resource/TextureManager.h"
#include "engine/event/RenderManager.h"

class BlankObject {
public:
    BlankObject() = delete;
    explicit BlankObject(EventManager& eventManager, TextureManager& textureManager, RenderManager& renderManager);

    void onPressEvent(const KeyPressedEvent& event);
    void onReleaseEvent(const KeyReleasedEvent& event);
    void onKeysHeldEvent(const KeysHeldEvent& event);

    void update();
    
private:
    EventListenerComponent eventListener_;
    TextureManager& textureManager_;
    RenderManager& renderManager_;
    Texture* myTexture_;
    float x_ = 100.0f;
    float y_ = 100.0f;
};
