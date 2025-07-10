#pragma once

#include <SDL3/SDL.h>
#include <functional>
#include <unordered_map>
#include <vector>

#include "EventInterface.h"

using EventCallback = std::function<void(const SDL_Event&)>;

class EventManager{
private:
    SDL_Event event;
    std::vector<EventTypes*> keyboardListeners;

public:
    EventManager();
    ~EventManager();
    
    const SDL_Event& getEvent() { return event; }

    void pollEvents();
    void addkeyboardListener(EventType listener);
    void removeKeyboardListener(EventType listener)
};