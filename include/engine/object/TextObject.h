#pragma once

#include "engine/object/Object.h"
#include "engine/component/TextComponent.h"
#include "engine/component/TransformComponent.h"
#include <memory>

class TextObject : public Object {
public:
    TextObject(std::unique_ptr<Text> text, float x, float y);

    void setText(const std::string& newText);
    void setColor(SDL_Color newColor);
    void setPosition(float x, float y);

private:
    TextComponent* textComponent_;
    TransformComponent* transform_;
};
