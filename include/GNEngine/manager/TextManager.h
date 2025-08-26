#pragma once
#include "../GNEngine_API.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <map>
#include <memory>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <format>
#include <functional> // For std::hash<std::filesystem::path> (if using unordered_map)

#include "GNEngine/core/Text.h"

/*
 * @brief 폰트를 로드하고 Text 객체를 생성 및 관리하는 클래스.
*/ 
class GNEngine_API TextManager {
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
     * @param filePath 폰트 파일 경로 (폰트의 고유 ID로 사용됨)
     * @param fontPointSize 폰트 크기 (포인트 단위)
     * @return 성공 시 true, 실패 시 false
     */
    bool loadFont(const std::filesystem::path& filePath, int fontPointSize);

    /*
     * @brief 폰트 크기를 Point 단위로 변경함.
     * @param filePath 폰트 파일 경로 (폰트의 고유 ID로 사용됨)
     * @param sizePoint 변경할 폰트 크기 (포인트 단위)
     * @return 성공 시 true, 실패 시 false
     */
    bool setFontSizePt(const std::filesystem::path& filePath, int fontPointSize);

    /*
     * @brief 지정된 파일 경로, 내용, 색상으로 Text 객체를 생성함.
     * @param filePath 사용할 폰트의 파일 경로 (폰트의 고유 ID로 사용됨)
     * @param text 표시할 텍스트 내용
     * @param color 텍스트 색상
     * @param isMultiline true: 개행 문자를 처리.  false: 개행 문자를 무시함.
     * @return 생성된 Text 객체에 대한 std::unique_ptr
     */
    std::unique_ptr<Text> createText(const std::filesystem::path& filePath, const std::string& text, SDL_Color color, bool enableMultiline = false, bool enableNewline = false, int wrapWidth = 0, int maxHeight = 0);

    /**
     * @brief 파일 경로로부터 텍스트를 불러옴.
     * @param filePath 텍스트 파일의 경로.
     * @return 파일의 내용을 담은 std::string. 파일이 존재하지 않거나 읽을 수 없는 경우 예외를 발생시킴.
    */
    std::string loadTextFromFile(const std::filesystem::path& filePath);

private:
    SDL_Renderer* renderer_;
    std::map<std::filesystem::path, TTF_Font*> fontMap_;
};



