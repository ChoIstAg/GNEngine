#pragma once

#include "engine/component/Component.h"
#include "engine/resource/text/Text.h"
#include "engine/component/TransformComponent.h"
#include <memory>

class TextComponent : public Component {
public:
    TextComponent(std::unique_ptr<Text> text, TransformComponent& transform);

    void render();
    void setText(const std::string& newText);
    void setColor(SDL_Color newColor);

private:
    std::unique_ptr<Text> text_;
    TransformComponent* transform_;
};
