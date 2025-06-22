#include "TextureObj.hpp"
#include <iostream>

/* constructor */
TextureObj::TextureObj(SDL_Renderer* rawRenderer, std::string filePath, int frame, bool preLoad, float x, float y, float scaleX, float scaleY) 
: rawRenderer(rawRenderer), filePath(filePath), frame(frame), x(x), y(y), scaleX(scaleX), scaleY(scaleY){
    if(!rawRenderer){ SDL_Log("rawRenderer in TextureObj is erroneous: %s", SDL_GetError());}

    if(preLoad == 1){ /*If preLoad is True, automatically load texture*/
        std::cerr << this << "::load_texture()" << std::endl;
        if(!load_texture()){ /* if loading is fail */
            SDL_Log("TextureObj %s : load_texture failed: %s", this , SDL_GetError());
        }
        std::cerr << "TextureObj " << this << "is completely loaded" << std::endl;
    }
}

/* destructor */
TextureObj::~TextureObj() {
    // Free objects
    if(tmpSurface){SDL_DestroySurface(tmpSurface);}
    if(tmpTexture){SDL_DestroyTexture(tmpTexture);}
    for(SDL_Texture* tex : textureVec){ SDL_DestroyTexture(tex);}

    std::cerr << "TextureObj " << this << " is successfully destroyed" << std::endl;
}


bool TextureObj::load_texture(){

    if(frame == true){ /* Check if this object has animation*/
        if(filePath.substr(filePath.size()-4, 4) == ".gif"){ /* Check if the file extrension is "gif"*/
            /* set key to the rest subtracted from the end of the file path*/
            const char* animationName = filePath.substr(0, filePath.size()-4).c_str();

            animationMap[animationName] = IMG_LoadAnimation(filePath.c_str());
            if(!animationMap[animationName]){
                SDL_Log("TextureObj %s : IMG_LoadAnimation failed: %s", this, SDL_GetError());
                return false;
            }
        }

        else if(filePath.substr(filePath.size()-4, 4) == ".png"){
            SDL_Surface* sheetSurface = IMG_Load(filePath.c_str()); /* spread sheet that format is png */
            if (!sheetSurface) {
                SDL_Log("TextureObj %s : IMG_Load failed: %s", this, SDL_GetError());
                return false;
            }

            // Assume horizontal sprite sheet: frame count = number of frames, each frame is width/frame wide
            int frameWidth = sheetSurface->w / frame; /* width per each frame*/
            int frameHeight = sheetSurface->h; /* height of entire sheet*/

            for(int i = 0; i < frame; ++i) {
                SDL_Rect srcRect = { i * frameWidth, 0, frameWidth, frameHeight }; /* to cutting only the shape of the square in the sprite sheet*/
                SDL_Surface* frameSurface = SDL_CreateSurface(frameWidth, frameHeight, sheetSurface->format);
                if (!frameSurface) {
                    SDL_Log("TextureObj %s : SDL_CreateSurface failed: %s", this, SDL_GetError());
                    SDL_DestroySurface(sheetSurface);
                    return false;
                }

                SDL_BlitSurface(sheetSurface, &srcRect, frameSurface, NULL); /* cut sprite sheet, and copy it, and phast to frameSurace */

                tmpTexture = SDL_CreateTextureFromSurface(rawRenderer, frameSurface);
                if (!tmpTexture) {
                    SDL_Log("%s : SDL_CreateTextureFromSurface failed: %s", this, SDL_GetError());
                    SDL_DestroySurface(frameSurface);
                    SDL_DestroySurface(sheetSurface);
                    return false; 
                }
                textureVec.push_back(tmpTexture);
                SDL_DestroySurface(frameSurface);
            }
            SDL_DestroyTexture(tmpTexture);
            SDL_DestroySurface(sheetSurface);
        }
    }

    else{
        /* Check if the file extension is ".bmp" or ".png" and create SDL_Surface*/
        if(filePath.substr(filePath.size() - 4, 4) == ".bmp"){
            tmpSurface = SDL_LoadBMP(filePath.c_str());
            if(tmpSurface == NULL){
                SDL_Log("TextureObj %s : SDL_LoadBMP failed: %s", this, SDL_GetError());
                return false;
            }
        }
        else if(filePath.substr(filePath.size() - 4, 4) == ".png"){
            tmpSurface = IMG_Load(filePath.c_str());
            if(tmpSurface == NULL){
                SDL_Log("TextureObj %s : IMG_Load failed: %s", this, SDL_GetError());
                return false;
            }
        }
        else {
            SDL_Log("TextureObj %s : Unsupported file format: %s", this, filePath.c_str());
            return false;
        }

        /* add texture to end*/
        textureVec.push_back(SDL_CreateTextureFromSurface(rawRenderer, tmpSurface));
        if(textureVec.back() == nullptr){
            SDL_Log("Error occured in texture: %s", SDL_GetError());
        }
        return true;
    }

    /*get texture size and assign to destination rectangle*/
    SDL_GetTextureSize(textureVec.back(), &width, &height);
    dstRect = {x, y, width * scaleX, height * scaleY};
    if(dstRect.w == 0 || dstRect.h == 0){
        SDL_Log("Error occured in dstRect: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool TextureObj::render_texture(){

}


SDL_Texture* TextureObj::get_texture(){
    return textureVec.back();
}

void TextureObj::move_texture(float x, float y) {
    dstRect.x += x;
    dstRect.y += y;
}

/* return position. if position is 'x' -> return x or 'y' -> return y or 'w' -> return width or 'h' -> return height.*/
float TextureObj::get_position(char position){
    if(position == 'x') {return dstRect.x;}
    else if(position == 'y') {return dstRect.y;}
    else if(position == 'w') {return dstRect.w;}
    else if(position == 'h') {return dstRect.h;}
    return false;
}

void TextureObj::rotate_texture(float angle){
    //SDL_RenderTextureRotated(rawRenderer, texture, NULL, &dstRect, {float(dstRect.x/2), float(dstRect.y/2)}, angle, 0);  
}