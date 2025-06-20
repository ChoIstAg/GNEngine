#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <map>


class TextureObj {
private:    
    SDL_Renderer* rawRenderer; /* raw renderer used in main */
    bool preLoad = 1; /* if preLoad is true, Texture is loaded when this class is allocated*/
    int frame = 0; /* frame number of animation (0 -> mere texture)*/
    
    /* It is animation struct if file is gif. */
    std::map<std::string, IMG_Animation*> animationMap; /* {animation name : value} */

    std::vector<SDL_Texture*> textureVec;
    std::string filePath;
    SDL_FRect dstRect; // Destination rectangle for rendering
    
    SDL_Surface* tmpSurface= nullptr; /* */
    SDL_Texture* tmpTexture = nullptr;

    float x, y, scaleX, scaleY, width, height;
    /* width will be setted to scaleX * w of surface and height is too.*/

public:   
    TextureObj(SDL_Renderer*, std::string filePath, int frame, bool preLoad, 
        float x, float y, float scaleX, float scaleY);        
    ~TextureObj();

    bool initialize(); /* initialize variables */
    bool load_texture(); 


    bool render_texture(); /* render a static texture to use rawRenderer*/
    bool update_animation(const char* animationName); /* update animation to use*/
    bool render_animation(const char* animationName); /* */
    

    void move_texture(float x, float y);
    void rotate_texture(float angle); /* rotate clockwise*/
    SDL_Texture* get_texture();
    float get_position(char position); /* ex: 'x' -> return x. (x,y,w,h)*/
};