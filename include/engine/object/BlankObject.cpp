#include "BlankObject.h"
#include <iostream>

BlankObject::BlankObject(EventManager& eventManager)
    : eventListener_(eventManager) { 

    eventListener_.addListener<TestEvent>( [this](const TestEvent& event) { std::cout << "TestEvent received: " << event.message << std::endl; } );
}

void BlankObject::onPressEvent(const KeyPressedEvent& event) {
    std::cout << "Key pressed: " << event.keyCode << std::endl;
}