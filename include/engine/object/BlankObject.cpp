#include "BlankObject.h"
#include <iostream>

BlankObject::BlankObject(EventManager& eventManager)
    : eventListener_(eventManager)
{

    eventListener_.addListener<KeyPressedEvent>([this](const KeyPressedEvent& event) {
        onPressEvent(event);
    });
}

void BlankObject::onPressEvent(const KeyPressedEvent& event) {
    std::cout << "TestEvent received: " << event.keyCode << std::endl;
}