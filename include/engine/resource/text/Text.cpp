#include "Text.h"
#include <stdexcept>

Text::Text(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color)
    : renderer_(renderer), font_(font), text_(text), color_(color), texture_(nullptr) {
    if (!renderer || !font) {
        throw std::runtime_error("Renderer or font is null");
    }
    createTexture();
}

Text::~Text() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
    }
}

void Text::render(float x, float y) {
    dstRect_.x = x;
    dstRect_.y = y;
    SDL_RenderTexture(renderer_, texture_, nullptr, &dstRect_);
}

void Text::setText(const std::string& newText) {
    text_ = newText;
    createTexture();
}

void Text::setColor(SDL_Color newColor) {
    color_ = newColor;
    createTexture();
}

int Text::getWidth() const {
    return static_cast<int>(dstRect_.w);
}

int Text::getHeight() const {
    return static_cast<int>(dstRect_.h);
}

void Text::createTexture() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font_, text_.c_str(), text_.length(), color_);
    if (!surface) {
        throw std::runtime_error("Failed to create surface from text");
    }

    texture_ = SDL_CreateTextureFromSurface(renderer_, surface);
    if (!texture_) {
        SDL_DestroySurface(surface);
        throw std::runtime_error("Failed to create texture from surface");
    }

    dstRect_.w = static_cast<float>(surface->w);
    dstRect_.h = static_cast<float>(surface->h);

    SDL_DestroySurface(surface);
}