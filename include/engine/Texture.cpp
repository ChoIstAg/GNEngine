#include "Texture.hpp"

/* Destructor of Texture to freeing texture*/
Texture::~Texture(){
    SDL_DestroyTexture(texture);
}