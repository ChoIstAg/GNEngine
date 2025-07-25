#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <map>
#include <memory>
#include "engine/resource/text/Text.h"

/*
 * @brief 폰트를 로드하고 Text 객체를 생성 및 관리하는 클래스임.
*/
class TextManager {
public:
    /*
     * @brief TextManager를 초기화함.
     * @param renderer 렌더링에 사용될 SDL_Renderer 포인터
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

private:
    SDL_Renderer* renderer;
    std::map<std::string, TTF_Font*> fontMap;
};