#include "engine/resource/Text.h"
#include <stdexcept>

Text::Text(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, bool enableMultiline, bool enableNewline, int wrapWidth, int maxHeight)
    : renderer_(renderer), font_(font), text_(text), color_(color), texture_(nullptr), enableMultiline(enableMultiline), enableNewline(enableNewline), wrapWidth_(wrapWidth), maxHeight_(maxHeight) {
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
    if (!texture_) return; // 텍스처가 없으면 렌더링하지 않음

    dstRect_.x = x;
    dstRect_.y = y;

    // maxHeight_가 설정되어 있고, 텍스처의 실제 높이보다 작으면 잘라서 렌더링
    SDL_FRect srcRect = {0, 0, dstRect_.w, dstRect_.h};
    if (maxHeight_ > 0 && dstRect_.h > maxHeight_) {
        dstRect_.h = static_cast<float>(maxHeight_);
        srcRect.h = static_cast<float>(maxHeight_);
    }

    SDL_RenderTexture(renderer_, texture_, &srcRect, &dstRect_);
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

void Text::setMultiline(bool multiline) {
    if (enableMultiline != multiline) {
        enableMultiline = multiline;
        createTexture();
    }
}

void Text::setNewline(bool enable) {
    if (enableNewline != enable) {
        enableNewline = enable;
        createTexture();
    }
}

void Text::setWrapWidth(int width) {
    if (wrapWidth_ != width) {
        wrapWidth_ = width;
        createTexture();
    }
}

void Text::setMaxHeight(int height) {
    if (maxHeight_ != height) {
        maxHeight_ = height;
        createTexture();
    }
}

void Text::createTexture() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr; // 텍스처를 파괴한 후 nullptr로 설정
    }

    SDL_Surface* surface = nullptr;
    std::string processedText = text_;

    if (enableMultiline) {

        surface = TTF_RenderText_Blended_Wrapped(font_, processedText.c_str(), processedText.length(), color_, wrapWidth_); 
    } else if (enableNewline) {
        // enableMultiline이 false이고 enableNewline이 true이면 첫 번째 줄만 렌더링
        size_t newlinePos = processedText.find('\n');
        if (newlinePos != std::string::npos) {
            processedText = processedText.substr(0, newlinePos);
        }
        surface = TTF_RenderText_Blended(font_, processedText.c_str(), processedText.length(), color_);
    } else {
        // 둘 다 false이면 한 줄로 렌더링 (개행 문자 무시)
        surface = TTF_RenderText_Blended(font_, processedText.c_str(), processedText.length(), color_);
    }

    if (!surface) {
        SDL_Log("Text::createTexture - Failed to create surface from text(%p): %s", this, SDL_GetError());
        return;
    }

    //SDL_Log("Text::createTexture - Surface dimensions: w=%d, h=%d, pixels: %p, pitch: %d", surface->w, surface->h, (void*)surface->pixels, surface->pitch);

    texture_ = SDL_CreateTextureFromSurface(renderer_, surface);
    if (!texture_) {
        SDL_DestroySurface(surface);
        SDL_Log("Text::createTexture - Failed to create texture from surface: %s", SDL_GetError());
        return;
    }

    dstRect_.w = static_cast<float>(surface->w);
    dstRect_.h = static_cast<float>(surface->h);

    SDL_DestroySurface(surface);
}