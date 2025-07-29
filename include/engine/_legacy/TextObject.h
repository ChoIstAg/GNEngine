#pragma once

#include "engine/resource/text/Text.h"
#include "engine/component/TransformComponent.h"
#include <memory>

/*
 * @brief 텍스트를 화면에 표시하기 위한 객체.
*/
class TextObject {
public:

    /*
     * @brief TextObject의 생성자.
     * @param text Text 객체에 대한 std::unique_ptr
     * @param x 초기 x 좌표
     * @param y 초기 y 좌표
     */
    TextObject(std::unique_ptr<Text> text, float x, float y);
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