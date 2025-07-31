#pragma once

#include "engine/object/Object.h"
#include "engine/component/TransformComponent.h"
#include "engine/component/RenderComponent.h"
#include "engine/component/SoundComponent.h"
#include "engine/component/EventListenerComponent.h"
#include "engine/manager/event/EventManager.h"
#include "engine/manager/render/RenderManager.h"
#include "engine/manager/sound/SoundManager.h"
#include "engine/manager/resource/TextureManager.h"


class TestObject : public Object {
public:
    TestObject(EventManager& eventManager, TextureManager& textureManager, RenderManager& renderManager, SoundManager& soundManager);

    void update(float dt) override;

private:
    void onPressEvent(const KeyPressedEvent& event);
    void onKeysHeldEvent(const KeysHeldEvent& event);

    TransformComponent* transform_;
    RenderComponent* render_;
    SoundComponent* sound_;
    EventListenerComponent* eventListener_;
};
