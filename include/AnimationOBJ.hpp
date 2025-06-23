#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

class AnimationObj{
public:
    bool preLoad;

    Animation animations;
    int frameNum; /* frame number of animation (0 -> only one texture)*/
    
    float x, y, w, h;
    float scaleX, scaleY;

    bool updateAnimation(const char* animationName); /* update animation to use*/
    bool renderAnimation(const char* animationName); /*  */    

};

using Animation = struct Animation{
    int frame; /* number of frame*/
    std::vector<SDL_Texture*> textureVec;
    SDL_FRect* dstRect; /* Destination Rectangle. */
    int* delays; /* An array of the time between this texture and next texture */
};