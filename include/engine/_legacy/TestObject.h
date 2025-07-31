#pragma once

#include <memory>
#include "engine/manager/event/EventManager.h"
#include "engine/event/EventInterface.h"
#include "engine/manager/resource/TextureManager.h"
#include "engine/manager/render/RenderManager.h"
#include "engine/manager/sound/SoundManager.h"

#include "engine/component/EventListenerComponent.h"
#include "engine/component/SoundComponent.h"

class TestObject {
public:
    TestObject() = delete;
    explicit TestObject(EventManager& eventManager, TextureManager& textureManager, RenderManager& renderManager, SoundManager& soundManager);

    void onPressEvent(const KeyPressedEvent& event);
    void onReleaseEvent(const KeyReleasedEvent& event);
    void onKeysHeldEvent(const KeysHeldEvent& event);

    void update();
    void render();
    
private:
    TextureManager& textureManager_;
    RenderManager& renderManager_;
    SoundManager& soundManager_;

    EventListenerComponent eventListener_;
    std::unique_ptr<SoundComponent> hitSound_;

    Texture* texture_;
    float x_ = 100.0f;
    float y_ = 100.0f;
};
