#pragma once
#include <SDL3/SDL.h>

class Texture {
public:
    SDL_Texture* sdlTexture_;
    int width_;
    int height_;

    Texture(SDL_Texture* texture, int width, int height) : sdlTexture_(texture), width_(width), height_(height) {}

    ~Texture() {
        if (sdlTexture_) {
            SDL_DestroyTexture(sdlTexture_);
        }
    }

    Texture(const Texture&) = delete; /* 복사 방지 */
    Texture& operator=(const Texture&) = delete; /* 복사 대입 방지 */

    // 이동 생성 및 할당 허용
    Texture(Texture&& other) noexcept
        : sdlTexture_(other.sdlTexture_),
          width_(other.width_),
          height_(other.height_)
    {
        other.sdlTexture_ = nullptr;
        other.width_ = 0;
        other.height_ = 0;
    }

    Texture& operator=(Texture&& other) noexcept {
        if (this != &other) {
            if (sdlTexture_) {
                SDL_DestroyTexture(sdlTexture_);
            }
            sdlTexture_ = other.sdlTexture_;
            width_ = other.width_;
            height_ = other.height_;
            other.sdlTexture_ = nullptr;
            other.width_ = 0;
            other.height_ = 0;
        }
        return *this;
    }
};