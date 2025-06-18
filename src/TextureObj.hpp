#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>

class TextureObj {

private:

    SDL_Renderer* renderer; /* raw renderer*/
    bool preLoad = 1; /* if preLoad is true, Texture is loaded when this class is allocated*/
    int frame = 0; /* frame number of animation (0 -> mere texture)*/

    IMG_Animation* animation = nullptr;
    SDL_Surface* surface;
    std::vector<SDL_Texture*> textures;
    std::string filePath;
    SDL_FRect dstRect; // Destination rectangle for rendering
    
    float x, y, scaleX, scaleY, width, height;

public:   
    
    TextureObj(SDL_Renderer*, std::string filePath, int frame, bool preLoad, float x, float y, float scaleX, float scaleY);        
    ~TextureObj();

    bool load_texture();
    bool render_texture();
    void move_texture(float x, float y);
    void turn_texture(float angle); 
    std::vector<SDL_Texture*> get_texture();
    float get_position(char position); /* ex: 'x' -> return x. (x,y,w,h)*/
    
};  