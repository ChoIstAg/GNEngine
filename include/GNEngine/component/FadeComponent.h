#pragma once

#include "GNEngine/core/Component.h"
#include <SDL3/SDL_pixels.h>
#include <functional>

/* 페이드 상태를 나타내는 열거형 */
enum class FadeState {
    NONE,
    FADE_IN,
    FADE_OUT
};

/*
 * @brief 페이드 효과에 필요한 모든 데이터를 저장하는 컴포넌트.
 * @note 현재 생성시 파라미터 없음.
 */
struct FadeComponent : public Component {
    FadeState state = FadeState::NONE;
    float timer = 0.0f;
    float duration = 1.0f;
    SDL_Color color = { 0, 0, 0, 255 };
    float currentAlpha = 0.0f; // 현재 알파값. FadeSystem이 매 프레임 계산.
    std::function<void()> onComplete = nullptr; //TODO1 - void function point로 바꾸기. 현재 오버헤드 존재함
};

