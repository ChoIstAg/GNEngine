#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <unordered_map>

#include "config.h"

using Texture = class Texture{
public:
    SDL_Texture* texture;
    SDL_FRect dstRect;
    float scaleX, scaleY;

    ~Texture();
};

/*The Texture object.
Argument: bool preLoad  
... SDL_Renderer* rawRenderer_, bool preLoad_, std::string filePath_, 
float x_, float y_, float scaleX_, float scaleY_
*/
class TextureObj {
private:    
    SDL_Renderer* rawRenderer; /* Raw renderer actually used in main */
    bool preLoad = true; /* if preLoad is true, Texture is loaded when this class is allocated*/
    
    int count = 0; /* The number of texture */
    
    std::unordered_map<std::string, Texture*> textureMap; /* texture name : texture value*/

public:
    /* this args will be used for initializing*/
    TextureObj(bool preInit_, 
        bool preLoad_, std::string filePath_, SDL_Renderer* rawRenderer_, 
        float x_, float y_, float scaleX_, float scaleY_);
    ~TextureObj();

    bool initialize(bool preLoad_, std::string filePath_, SDL_Renderer* rawRenderer_, float x_, float y_, float scaleX_, float scaleY_);
    bool loadTexture(const std::string& filePath_, float x_, float y_, float scaleX_, float scaleY_); 
    //bool setPosition(float x_, float y_);

    bool renderTexture(std::string fileName); /* render a static texture to use rawRenderer*/
    void moveTexture(std::string fileName_, float x_, float y_);
    void rotateTexture(std::string fileName_, float angle); /* rotate clockwise */
    SDL_Texture* getTexture(std::string fileName);
    float getPosition(std::string fileName_, char position); /* ex: ('x') -> return x. (x,y,w,h)*/
};