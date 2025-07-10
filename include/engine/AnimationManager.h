#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "Texture.hpp"

class AnimationManager{
public:
    bool preInit;

    std::unordered_map<std::string, Animation*> animationMap;
    int frameNum; /* frame number of animation (0 -> only one texture)*/
    
    float x, y, w, h;
    float scaleX, scaleY;

    bool loadAnimation(std::string filePath);
    bool updateAnimation(const char* animationName); /* update animation to use*/
    bool renderAnimation(const char* animationName); /*  */    

};

using Animation = struct Animation{
    int frameNum; /* number of frame*/
    Texture texture;
    int* delays; /* An array of the time between this texture and next texture */
};