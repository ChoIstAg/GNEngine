#include "TextManager.hpp"

/* constructor*/
TextManager::TextManager(SDL_Renderer* renderer){

}

/* distructor*/
TextObject::~TextObject(){
        if(font) {TTF_CloseFont(font);}
        if(textSurface) {SDL_DestroySurface(textSurface);}
        if(textTexture) {SDL_DestroyTexture(textTexture);}
}

/* load txt file*/
TextManager::load_textfile(const std::string& filepath){
    std::ifstream file(filepath);
    if(!file.is_open()) {
        std::cerr << "can't open file: %s " << filepath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

