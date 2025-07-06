#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <memory> // For std::unique_ptr

// Custom deleter for SDL_Texture
struct SDL_Texture_Deleter {
    void operator()(SDL_Texture* texture) const {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }
};

// Use unique_ptr with custom deleters for automatic resource management
using UniqueSDL_Texture = std::unique_ptr<SDL_Texture, SDL_Texture_Deleter>;

/* A Texture. */
class Texture{
public:
    // Constructor to take an SDL_Texture* and its properties
    Texture(SDL_Texture* sdlTexture, SDL_FRect dstRect, float scaleX, float scaleY);

    // Getters
    SDL_Texture* getSDLTexture() const { return m_sdlTexture.get(); }
    SDL_FRect& getDstRect() { return m_dstRect; } // Return by reference to allow modification
    float getScaleX() const { return m_scaleX; }
    float getScaleY() const { return m_scaleY; }
    bool setVisible(bool enable);
    bool getVisible() { return m_visible; }

private:
    UniqueSDL_Texture m_sdlTexture;
    SDL_FRect m_dstRect;
    float m_scaleX, m_scaleY;
    bool m_visible = false;
};  