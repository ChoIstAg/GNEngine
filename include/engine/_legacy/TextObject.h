#pragma once

#include "engine/resource/text/Text.h"
#include <memory>

/*
 * @brief 텍스트를 게임 월드에 표시하기 위한 게임 오브젝트 클래스임.
*/
class TextObject {
public:
    /*
     * @brief TextObject를 생성함.
     * @param text 렌더링할 Text 객체에 대한 소유권을 가진 포인터
     * @param x 화면에 표시될 x 좌표
     * @param y 화면에 표시될 y 좌표
     */
    TextObject(std::unique_ptr<Text> text, float x, float y);

    /*
     * @brief TextObject를 파괴함.
    */
    ~TextObject();

    /*
     * @brief 매 프레임 호출되어 텍스트를 렌더링함.
     */
    void render();

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
     * @brief 오브젝트의 위치를 설정함.
     * @param x 새로운 x 좌표
     * @param y 새로운 y 좌표
     */
    void setPosition(float x, float y);

private:
    std::unique_ptr<Text> text;
    float x, y;
};