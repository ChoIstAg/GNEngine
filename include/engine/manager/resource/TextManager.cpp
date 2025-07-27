#include "TextManager.h"

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
}

bool TextManager::loadFont(const std::string& id, const std::string& filePath, int fontSize) {
    TTF_Font* font = TTF_OpenFont(filePath.c_str(), fontSize);
    if (!font) {
        SDL_Log("Failed to load font %s: %s", filePath.c_str(), SDL_GetError());
        return false;
    }

    fontMap_[id] = font; /* 폰트 map에 저장 */
    return true;
}

std::unique_ptr<Text> TextManager::createText(const std::string& fontId, const std::string& text, SDL_Color color) {
    auto it = fontMap_.find(fontId);
    if (it == fontMap_.end()) { /* 만약 폰트를 찾지 못했다면 */
        return nullptr; 
    }
    return std::make_unique<Text>(renderer_, it->second, text, color);
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