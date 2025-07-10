#pragma once

#include <SDL3/SDL.h>

class EventManager{
    SDL_Event event;

    void updateEvent();
    const SDL_Event& getEvent() { return event; }

};