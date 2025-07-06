#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory> // For std::unique_ptr

#include "Texture.h"
#include "config.h"

// Custom deleter for SDL_Surface
struct SDL_Surface_Deleter {
    void operator()(SDL_Surface* surface) const {
        if (surface) {
            SDL_DestroySurface(surface);
        }
    }
};

// Use unique_ptr with custom deleters for automatic resource management
using UniqueSDL_Surface = std::unique_ptr<SDL_Surface, SDL_Surface_Deleter>;

/*The Texture object.
Argument: 
*/
class TextureManager {
private:    
    SDL_Renderer* rawRenderer_; /* Raw renderer actually used in main */
    bool preLoad_ = true; /* if preLoad is true, Texture is loaded when this class is allocated*/
    int count_ = 0; /* The number of texture */
    std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap_; /* Normal texture name : texture value*/
    std::unordered_map<std::string, std::unique_ptr<Texture>> backgroundMap_;

public:
    TextureManager();
    ~TextureManager();

    bool initialize(SDL_Renderer* rawRenderer);
    bool loadTexture(const std::string& filePath, float x, float y, float scaleX, float scaleY); 
    //bool setPosition(float x, float y);

    bool renderTexture(const std::string& filePath); /* render a static texture to use rawRenderer*/
    bool renderTextureAll();
    void moveTexture(const std::string& filePath, float x, float y);
    void rotateTexture(const std::string& filePath, float angle); /* rotate clockwise */

    Texture* getTexture(const std::string& filePath); // Returns raw pointer, but ownership remains with manager
    float getPosition(const std::string& filePath, char position); /* ex: ('x') -> return x. (x,y,w,h)*/

    bool getVisible(const std::string& filePath);
    bool setVisible(const std::string& filePath, bool enable);

    bool loadBackground(const std::string& filePath, int windowWidth, int windowHeight, float scaleX, float scaleY);
    bool renderBackground(const std::string& filePath);
};