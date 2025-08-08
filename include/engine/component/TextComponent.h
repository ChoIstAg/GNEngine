#pragma once
#include "../../GNEngine_API.h"

#include <memory>

#include "engine/core/Component.h"
#include "engine/resource/Text.h"

class GNEngine_API TextComponent : public Component {
public:
    TextComponent(std::unique_ptr<Text> text);

    // 이동 생성자 및 이동 대입 연산자
    TextComponent(TextComponent&& other) noexcept = default;
    TextComponent& operator=(TextComponent&& other) noexcept = default;

    // 복사 생성자 및 복사 대입 연산자 삭제 (unique_ptr 멤버 때문)
    TextComponent(const TextComponent&) = delete;
    TextComponent& operator=(const TextComponent&) = delete;
    
    Text* getText() const { return text_.get(); }

private:
    std::unique_ptr<Text> text_;
};
