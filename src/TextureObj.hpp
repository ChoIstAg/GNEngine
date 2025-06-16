#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>

class TextureObj {

private:

    SDL_Renderer* renderer;
    bool preLoad = 1;
    bool fps = 0;

    // 
    SDL_Surface* surface;
    SDL_Texture* texture;
    std::string filePath;
    SDL_FRect dstRect; // Destination rectangle for rendering
    
    float x, y, scaleX, scaleY, width, height;
    
public:   
    
    TextureObj(SDL_Renderer*, std::string filePath, bool fps, bool preLoad, float x, float y, float scaleX, float scaleY);        
    ~TextureObj();

    bool load_texture();
    bool render_texture();
    void move_texture(float x, float y); /* duration == 1->up, 2->down, 3->right, 4->left*/
    void turn_texture(float angle);
    SDL_Texture* get_texture();
    float get_position(char position);
    void cout_test();
};  