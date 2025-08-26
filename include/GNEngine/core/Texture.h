#pragma once
#include "../GNEngine_API.h"

#include <SDL3/SDL.h>

/* @brief Texture 湲곕낯 ?띿뒪泥?
 * @param texture SDL_Texture*
 * @param width(0)
 * @patam height(0)
*/
class GNEngine_API Texture {
public:
    SDL_Texture* sdlTexture_;
    int width_;
    int height_;

    Texture(SDL_Texture* texture, int width=0, int height=0) : sdlTexture_(texture), width_(width), height_(height) {}

    ~Texture() {
        if (sdlTexture_) {
            SDL_DestroyTexture(sdlTexture_);
        }
    }

    Texture(const Texture&) = delete; /* 蹂듭궗 ??젣 */
    Texture& operator=(const Texture&) = delete; /* 蹂듭궗 ?????젣 */

    /* ?대룞 ?앹꽦 諛??좊떦留??덉슜 */
    Texture(Texture&& other) noexcept
        : sdlTexture_(other.sdlTexture_),
          width_(other.width_),
          height_(other.height_)
    {
        other.sdlTexture_ = nullptr;
        other.width_ = 0;
        other.height_ = 0;
    }

    /* ??낆떆 ?뚯쑀沅뚯쓣 ?대룞?? */
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


