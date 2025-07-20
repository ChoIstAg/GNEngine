#pragma once

#include "engine/event/EventManager.h"
#include "engine/event/EventListnerComponent.h"

class BlankObject {
public:
    BlankObject() = delete;
    explicit BlankObject(EventManager& eventManager);

    void onPressEvent(const KeyPressedEvent& event);

private:
    EventListenerComponent eventListener_;
};
