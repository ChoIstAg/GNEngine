#include "TextureObj.hpp"
#include <iostream>

/* Destructor of Texture to freeing texture*/
Texture::~Texture(){
    SDL_DestroyTexture(texture);
}

/*The Texture object.
Argument: bool preLoad  
... bool preLoad_, std::string filePath_, SDL_Renderer* rawRenderer_, 
float x_, float y_, float scaleX_, float scaleY_
*/
TextureObj::TextureObj(bool preInit_, 
    bool preLoad_, std::string filePath_, SDL_Renderer* rawRenderer_, 
    float x_, float y_, float scaleX_, float scaleY_){
        
    if(preInit_){ initialize(preLoad_, filePath_, rawRenderer_, x_, y_, scaleX_, scaleY_); }
}

/* Destructor */
TextureObj::~TextureObj() {
    // Free objects
    textureMap.clear();
    std::cerr << "TextureObj " << this << " is successfully destroyed" << std::endl;
}

/* Initialize Texture and load texture if preLoad is True */
bool TextureObj::initialize(bool preLoad_, std::string filePath_, SDL_Renderer* rawRenderer_, float x_, float y_, float scaleX_, float scaleY_){
    if(rawRenderer_ != nullptr){ 
        rawRenderer = rawRenderer_;
        if(!rawRenderer){ SDL_Log("rawRenderer in TextureObj is erroneous: %s", SDL_GetError());}
    }
    //filePath(filePath), frame(frame), x(x), y(y), scaleX(scaleX), scaleY(scaleY)


    if(preLoad == 1){ /*If preLoad is True, automatically load texture*/
        if(!loadTexture(filePath_, x_, y_, scaleX_, scaleY_)){ /* if loading is fail */
            SDL_Log("TextureObj %s : load_texture failed: %s", this , SDL_GetError());
        }
        std::cerr << "TextureObj(filePath:" << filePath_ << ")" << this << "is completely loaded" << std::endl;
    }
    return true;
}   

/* Loating texture(bmp, png) or animation(gif, png(sprite sheet).*/
bool TextureObj::loadTexture(const std::string& filePath_, float x_, float y_, float scaleX_, float scaleY_){
    SDL_Surface* tmpSurface_;
    std::string fileName_ = filePath_.substr(0, filePath_.size() - 4);
    std::string filePath__ = filePath_.substr(filePath_.size() - 4, 4); /* file name except extention*/
    
    /* Check if the file extension is ".bmp" or ".png" and create SDL_Surface*/
    if(filePath__ == ".bmp"){
        tmpSurface_ = SDL_LoadBMP((IMAGE_ASSET_ROOT_PATH + filePath_).c_str());
        if(tmpSurface_ == NULL){
            SDL_Log("TextureObj %s : SDL_LoadBMP failed: %s", this, SDL_GetError());
            return false;
        }
    }
    else if(filePath__ == ".png"){
        tmpSurface_ = IMG_Load((IMAGE_ASSET_ROOT_PATH + filePath_).c_str());
        if(tmpSurface_ == nullptr){
            SDL_Log("TextureObj %s : IMG_Load failed: %s", this, SDL_GetError());
            return false;
        }
    }
    else {
        SDL_Log("TextureObj %s : Unsupported file format: %s", this, (PROJECT_ROOT_PATH + filePath_).c_str());
        return false;
    }

    /* create texture */
    textureMap[fileName_] = new Texture{ //segfloat 고친것
        SDL_CreateTextureFromSurface(rawRenderer, tmpSurface_),
        {x_, y_, tmpSurface_->w * scaleX_, tmpSurface_->h * scaleY_},
        scaleX_, scaleY_
    };
    // *textureMap[fileName_] = {SDL_CreateTextureFromSurface(rawRenderer, tmpSurface_), 
    //     {x_, y_, tmpSurface_->w * scaleX_, tmpSurface_->h * scaleY_},  /* set width to w * scaleX, and h too.*/
    //     scaleX_, scaleY_}; //-> Error occured by segfloat

    if(textureMap[fileName_] == nullptr){
        SDL_Log("Error occured in texture: %s", SDL_GetError());
        return false;
    }   

    count++;
    return true;
}

bool TextureObj::renderTexture(std::string fileName){
    if(!SDL_RenderTexture(rawRenderer, textureMap[fileName]->texture, NULL, &textureMap[fileName]->dstRect)){
        SDL_Log("TextureObj %s : Error is occured in SDL_RenderTexture: %s", this, SDL_GetError());
        return false;
    }
    return true;
}


SDL_Texture* TextureObj::getTexture(std::string fileName_){
    return textureMap[fileName_]->texture;
}

void TextureObj::moveTexture(std::string fileName_, float x_, float y_) {
    textureMap[fileName_]->dstRect.x += x_;
    textureMap[fileName_]->dstRect.y += y_;
}

/* return position. if position is 'x' -> return x or 'y' -> return y or 'w' -> return width or 'h' -> return height.*/
float TextureObj::getPosition(std::string fileName_, char position){
    if(position == 'x') {return textureMap[fileName_]->dstRect.x;}
    else if(position == 'y') {return textureMap[fileName_]->dstRect.y;}
    else if(position == 'w') {return textureMap[fileName_]->dstRect.w;}
    else if(position == 'h') {return textureMap[fileName_]->dstRect.h;}
    return false;
}

void TextureObj::rotateTexture(std::string fileName, float angle){
    //SDL_RenderTextureRotated(rawRenderer, texture, NULL, &dstRect, {float(dstRect.x/2), float(dstRect.y/2)}, angle, 0);  
}