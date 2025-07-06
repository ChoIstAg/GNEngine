#include "Texture.h"

Texture::Texture(SDL_Texture* sdlTexture, SDL_FRect dstRect, float scaleX, float scaleY)
    : m_sdlTexture(sdlTexture), m_dstRect(dstRect), m_scaleX(scaleX), m_scaleY(scaleY) {}

bool Texture::setVisible(bool enable) {
    m_visible = enable;
    return true; // Assuming visibility toggle is always successful
}