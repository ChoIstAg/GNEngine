#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <map>
#include <memory>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <format>

#include "engine/resource/text/Text.h"

/*
 * @brief 폰트를 로드하고 Text 객체를 생성 및 관리하는 클래스.
*/
class TextManager {
public:
    /*
     * @brief TextManager를 초기화함.
     * @param renderer 이미 SDL_CreateRenderer()로 생성을 마친 SDL_Renderer의 포인터 주소값.
     */
    TextManager(SDL_Renderer* renderer);

    /*
     * @brief TextManager를 파괴하고 로드된 모든 폰트를 정리함.
    */
    ~TextManager();

    /*
     * @brief 지정된 경로와 크기로 폰트를 로드함.
     * @param id 폰트를 식별할 고유 ID
     * @param filePath 폰트 파일 경로
     * @param fontSize 폰트 크기
     * @return 성공 시 true, 실패 시 false
     */
    bool loadFont(const std::string& id, const std::string& filePath, int fontSize);

    /*
     * @brief 지정된 ID, 내용, 색상으로 Text 객체를 생성함.
     * @param fontId 사용할 폰트의 ID
     * @param text 표시할 텍스트 내용
     * @param color 텍스트 색상
     * @return 생성된 Text 객체에 대한 std::unique_ptr
     */
    std::unique_ptr<Text> createText(const std::string& fontId, const std::string& text, SDL_Color color);

    /**
     * @brief 파일 경로로부터 텍스트를 불러옴.
     * @param filePath 텍스트 파일의 경로.
     * @return 파일의 내용을 담은 std::string. 파일이 존재하지 않거나 읽을 수 없는 경우 예외를 발생시킴.
    */
    std::string loadTextFromFile(const std::filesystem::path& filePath);

private:
    SDL_Renderer* renderer_;
    std::map<std::string, TTF_Font*> fontMap_;
};