#include "TextManager.hpp"

TextManager::TextManager(SDL_Renderer* renderer){

}
TextManager::load_textfile(const std::string& filepath){
        std::ifstream file(filepath);
        if(!file.is_open()) {
            std::cerr << "파일을 열 수 없습니다: " << filepath << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }


TextObject::~TextObject(){
        if(font) {TTF_CloseFont(font);}
        if(textSurface) {SDL_DestroySurface(textSurface);}
        if(textTexture) {SDL_DestroyTexture(textTexture);}
}