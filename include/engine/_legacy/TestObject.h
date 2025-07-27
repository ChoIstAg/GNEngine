#pragma once

#include "engine/manager/event/EventManager.h"
#include "engine/component/EventListenerComponent.h"
#include "engine/event/EventInterface.h"
#include "engine/manager/resource/TextureManager.h"
#include "engine/manager/render/RenderManager.h"

class TestObject {
public:
    TestObject() = delete;
    explicit TestObject(EventManager& eventManager, TextureManager& textureManager, RenderManager& renderManager);

    void onPressEvent(const KeyPressedEvent& event);
    void onReleaseEvent(const KeyReleasedEvent& event);
    void onKeysHeldEvent(const KeysHeldEvent& event);

    void update();
    
private:
    EventListenerComponent eventListener_;
    TextureManager& textureManager_;
    RenderManager& renderManager_;
    Texture* texture_;
    float x_ = 100.0f;
    float y_ = 100.0f;
};
