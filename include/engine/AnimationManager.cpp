#include "AnimationManager.hpp"

bool AnimationManager::loadAnimation(std::string filePath){
        if(frame == true){ /* Check if this object has animation*/
        if(filePath_.substr(filePath_.size()-4, 4) == ".gif"){ /* Check if the file extrension is "gif"*/
            /* set key to the rest subtracted from the end of the file path*/
            const char* animationName_ = filePath_.substr(0, filePath_.size()-4).c_str();

            animationMap[animationName_] = IMG_LoadAnimation(filePath_.c_str());
            if(!animationMap[animationName_]){
                SDL_Log("TextureObj %s : IMG_LoadAnimation failed: %s", this, SDL_GetError());
                return false;
            }
            
            /* create texture using surface */
            for(int i = 0; i < frame; i++){
                textureVec.push_back(SDL_CreateTextureFromSurface(rawRenderer, animationMap[animationName_]->frames[i]));

            }
        }
        /* Check if the file is sprite sheet(png) */
        else if(filePath_.substr(filePath_.size()-4, 4) == ".png"){
            /* Load spread sheet that format is png */
            SDL_Surface* sheetSurface_ = IMG_Load(filePath_.c_str());
            if (!sheetSurface_) {
                SDL_Log("TextureObj %s : IMG_Load failed: %s", this, SDL_GetError());
                return false;
            }
            
            // Assume horizontal sprite sheet: frame count = number of frames, each frame is width/frame wide
            int frameWidth_ = sheetSurface_->w / frame; /* width per each frame*/
            int frameHeight_ = sheetSurface_->h; /* height of entire sheet*/
            
            for(int i = 0; i < frame; ++i) {
                SDL_Rect srcRect_ = { i * frameWidth_, 0, frameWidth_, frameHeight_ }; /* to cutting only the shape of the frame in the sprite sheet*/
                SDL_Surface* frameSurface_ = SDL_CreateSurface(frameWidth_, frameHeight_, sheetSurface_->format);
                if (!frameSurface_) {
                    SDL_Log("TextureObj %s : SDL_CreateSurface failed: %s", this, SDL_GetError());
                    SDL_DestroySurface(sheetSurface_);
                    return false;
                }

                SDL_BlitSurface(sheetSurface_, &srcRect_, frameSurface_, NULL); /* cut sprite sheet, and copy it, and phast to frameSurace */
                
                SDL_Texture* tmpTexture_ = SDL_CreateTextureFromSurface(rawRenderer, frameSurface_);
                if (!tmpTexture_) {
                    SDL_Log("%s : SDL_CreateTextureFromSurface failed: %s", this, SDL_GetError());
                    SDL_DestroySurface(frameSurface_);
                    SDL_DestroySurface(sheetSurface_);
                    return false; 
                }
                textureVec.push_back(tmpTexture_);
            }
            SDL_DestroySurface(frameSurface_);
        }
        SDL_DestroySurface(sheetSurface_);
        SDL_DestroyTexture(tmpTexture_);
    }
}
