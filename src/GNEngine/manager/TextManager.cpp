#include "GNEngine/manager/TextManager.h"
#include <iostream>

TextManager::TextManager(SDL_Renderer* renderer) : renderer_(renderer) {
    if (TTF_Init() == -1) {
        throw std::runtime_error("Failed to initialize SDL_ttf");
    }
}

TextManager::~TextManager() {
    for (auto const& [id, font] : fontMap_) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
    std::cerr << "TextManager "<< this << " is successfully destroyed and all fonts unloaded." << std::endl;
}

bool TextManager::loadFont(const std::filesystem::path& filePath, int fontPointSize) {
    // 이미 로드된 폰트인지 확인
    if (fontMap_.count(filePath)) {
        SDL_Log("Font already loaded: %s", filePath.string().c_str());
        return true; // 이미 로드되어 있다면 성공으로 간주
    }

    TTF_Font* font = TTF_OpenFont(filePath.string().c_str(), fontPointSize);
    if (!font) {
        SDL_Log("Failed to load font %s: %s", filePath.string().c_str(), SDL_GetError());
        return false;
    }

    fontMap_[filePath] = font; /* 폰트 map에 저장 */
    return true;
}

bool TextManager::setFontSizePt(const std::filesystem::path& filePath, int fontPointSize) {
    auto it = fontMap_.find(filePath);
    if (it == fontMap_.end()) {
        SDL_Log("setFontSizePt - Font not found: %s", filePath.string().c_str());
        return false;
    }
    if (!TTF_SetFontSize(it->second, fontPointSize)) {
        SDL_Log("setFontSizePt - Failed to set font size for %s: %s", filePath.string().c_str(), SDL_GetError());
        return false;
    }
    return true;
}

std::string TextManager::loadTextFromFile(const std::filesystem::path& filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::string errorMessage = std::format("File does not exist({}) : {}", filePath.string(), SDL_GetError());
        SDL_Log(errorMessage.c_str());
        return errorMessage;
    }

    std::ifstream file(filePath); /* 파일 열기 */
    if (!file.is_open()) {
        std::string errorMessage = std::format("Failed to open file({}) : {}", filePath.string(), SDL_GetError());
        SDL_Log(errorMessage.c_str());
        return errorMessage;
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); /* 파일의 모든 내용을 버퍼로 저장. */

    return buffer.str(); /* string으로 반환 */
}

TTF_Font* TextManager::getFont(const std::filesystem::path& filePath, int fontPointSize) {
    auto it = fontMap_.find(filePath);
    if (it != fontMap_.end()) {
        return it->second;
    }
    // If not found, try to load it
    if (loadFont(filePath, fontPointSize)) {
        return fontMap_.at(filePath);
    }
    return nullptr;
}


