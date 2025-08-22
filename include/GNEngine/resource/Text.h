#pragma once
#include "../../GNEngine_API.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

/*
 * @brief 텍스트 객체를 나타내는 클래스임.
 * TextManager에 의해 생성되고 관리됨.
*/ 
class GNEngine_API Text {
public:
    /*
     * @brief Text 객체를 생성함.
     * @param renderer 렌더링에 사용될 SDL_Renderer 포인터
     * @param font 텍스트 렌더링에 사용될 TTF_Font 포인터
     * @param text 표시할 텍스트 내용
     * @param color 텍스트 색상(Uint8 R, G, B, A)
     * @param enableMultiline 텍스트 내 개행 문자를 처리할지 여부
     * @param enableNewline 텍스트 내 개행 문자를 처리할지 여부 (첫 줄만 렌더링 시 사용)
     * @param wrapWidth 텍스트가 개행될 최대 너비 (0이면 너비 제한 없음)
     * @param maxHeight 텍스트가 렌더링될 최대 높이 (0이면 높이 제한 없음)
     */
    Text(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, bool enableMultiline = false, bool enableNewline = false, int wrapWidth = 0, int maxHeight = 0);

    /*
     * @brief 텍스트 내 개행 문자 처리 여부를 설정함.
     * @param multiline true면 개행 처리, false면 개행 무시
     */
    void setMultiline(bool multiline);

    /*
     * @brief 텍스트 내 개행 문자 처리 여부를 설정함 (첫 줄만 렌더링 시 사용).
     * @param enable true면 개행 문자 처리, false면 개행 문자 무시
     */
    void setNewline(bool enable);

    /*
     * @brief 텍스트의 최대 너비를 설정함. 이 너비를 초과하면 개행됨.
     * @param width 최대 너비 (픽셀 단위). 0이면 너비 제한 없음.
     */
    void setWrapWidth(int width);

    /*
     * @brief 텍스트의 최대 높이를 설정함. 이 높이를 초과하면 렌더링되지 않음.
     * @param height 최대 높이 (픽셀 단위). 0이면 높이 제한 없음.
     */
    void setMaxHeight(int height);

    ~Text();

    /*
     * @brief 지정된 위치에 텍스트를 렌더링함.
     * @param x 렌더링할 x 좌표
     * @param y 렌더링할 y 좌표
     */
    void render(float x, float y);

    /*
     * @brief 텍스트 내용을 변경함.
     * @param newText 덮어씌울 새로운 텍스트 내용
     */
    void setText(const std::string& newText);

    /*
     * @brief 텍스트 색상을 변경함.
     * @param newColor 새로운 텍스트 색상
     */
    void setColor(SDL_Color newColor);

    /*
     * @brief 텍스트의 너비를 가져옴.
     * @return 텍스트의 너비
     */
    int getWidth() const;

    /*
     * @brief 텍스트의 높이를 가져옴.
     * @return 텍스트의 높이
     */
    int getHeight() const;

    SDL_Texture* getSDLTexture() const { return texture_; }

private:
    /*
     * @brief 텍스트로부터 SDL_Texture를 생성함.
     * 내부적으로 호출되어 텍스트 내용이나 색상이 변경될 때 텍스처를 재생성함.
     */
    void createTexture();

    SDL_Renderer* renderer_;
    TTF_Font* font_;
    SDL_Texture* texture_; /* 렌더링할 텍스처 */
    std::string text_;
    SDL_FRect dstRect_; /* 렌더링할 위치 */
    SDL_Color color_;
    bool enableNewline; /* 개행 문자를 처리할지 여부. true면 개행 문자를 처리함. false면 개행문자를 무시하고 한줄로 내보냄. */
    bool enableMultiline; /* 여러 줄을 렌더링할지 여부*/
    int wrapWidth_;
    int maxHeight_;
};