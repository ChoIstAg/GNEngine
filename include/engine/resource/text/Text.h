#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

/*
 * @brief 텍스트 객체를 나타내는 클래스임.
 * TextManager에 의해 생성되고 관리됨.
*/
class Text {
public:
    /*
     * @brief Text 객체를 생성함.
     * @param renderer 렌더링에 사용될 SDL_Renderer 포인터
     * @param font 텍스트 렌더링에 사용될 TTF_Font 포인터
     * @param text 표시할 텍스트 내용
     * @param color 텍스트 색상
     */
    Text(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color);

    ~Text();

    /*
     * @brief 지정된 위치에 텍스트를 렌더링함.
     * @param x 렌더링할 x 좌표
     * @param y 렌더링할 y 좌표
     */
    void render(float x, float y);

    /*
     * @brief 텍스트 내용을 변경함.
     * @param newText 새로운 텍스트 내용
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

private:
    /*
     * @brief 텍스트로부터 SDL_Texture를 생성함.
     * 내부적으로 호출되어 텍스트 내용이나 색상이 변경될 때 텍스처를 재생성함.
     */
    void createTexture();

    SDL_Renderer* renderer_;
    TTF_Font* font_;
    SDL_Texture* texture_;
    std::string text_;
    SDL_FRect dstRect_;
    SDL_Color color_;
};