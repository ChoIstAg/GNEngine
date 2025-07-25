#include "TextManager.h"
#include <stdexcept>

TextManager::TextManager(SDL_Renderer* renderer) : renderer(renderer) {
    if (TTF_Init() == -1) {
        throw std::runtime_error("Failed to initialize SDL_ttf");
    }
}

TextManager::~TextManager() {
    for (auto const& [id, font] : fontMap) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
}

bool TextManager::loadFont(const std::string& id, const std::string& filePath, int fontSize) {
    TTF_Font* font = TTF_OpenFont(filePath.c_str(), fontSize);
    if (!font) {
        SDL_Log("Failed to load font %s: %s", filePath.c_str(), SDL_GetError());
        return false;
    }
    fontMap[id] = font;
    return true;
}

std::unique_ptr<Text> TextManager::createText(const std::string& fontId, const std::string& text, SDL_Color color) {
    auto it = fontMap.find(fontId);
    if (it == fontMap.end()) {
        return nullptr; /* 폰트를 찾지 못한 경우 */
    }
    return std::make_unique<Text>(renderer, it->second, text, color);
}