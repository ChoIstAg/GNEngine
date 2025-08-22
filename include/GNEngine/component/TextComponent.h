#pragma once
#include "../../GNEngine_API.h"

#include <string>
#include <filesystem>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

#include "GNEngine/core/Component.h"
#include "GNEngine/core/RenderLayer.h"

/*
 * @class TextComponent
 * @brief 렌더링할 텍스트에 대한 모든 정보를 담는 데이터 컴포넌트임.
 *        이 컴포넌트 자체는 로직을 가지지 않으며, RenderSystem이 이 정보를 사용해
 *        텍스처를 생성하고 화면에 렌더링함.
 * @param text 렌더링할 텍스트 문자열.
 * @param fontPath 사용할 폰트 파일의 경로.
 * @param fontSize 폰트의 크기.
 * @param color 텍스트의 색상 (SDL_Color).
 * @param layer(RenderLayer::UI) 렌더링 레이어.
 */
struct GNEngine_API TextComponent : public Component {
    std::string text;
    std::filesystem::path fontPath;
    int fontSize;
    SDL_Color color;
    RenderLayer layer = RenderLayer::UI;
    bool isDirty = true; // 내용이 변경되어 텍스처를 다시 만들어야 하는지 여부

    // RenderSystem이 생성하고 관리하는 텍스처 캐시
    SDL_Texture* texture = nullptr;
    int textureWidth = 0;
    int textureHeight = 0;

    TextComponent(std::string text, std::filesystem::path fontPath, int fontSize, SDL_Color color, RenderLayer layer = RenderLayer::UI)
        : text(std::move(text)), fontPath(std::move(fontPath)), fontSize(fontSize), color(color), layer(layer) {}

    // 텍스트를 변경하고 dirty 플래그를 설정하는 헬퍼 함수
    void setText(std::string newText) {
        if (text != newText) {
            text = std::move(newText);
            isDirty = true;
        }
    }
};

