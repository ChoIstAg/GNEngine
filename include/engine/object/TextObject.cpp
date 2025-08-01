#include "TextObject.h"

TextObject::TextObject(std::unique_ptr<Text> text, float x, float y) {
    transform_ = &addComponent<TransformComponent>(x, y);

    textComponent_ = &addComponent<TextComponent>(std::move(text), *transform_);
}

void TextObject::setText(const std::string& newText) {
    textComponent_->setText(newText);
}

void TextObject::setColor(SDL_Color newColor) {
    textComponent_->setColor(newColor);
}

void TextObject::setPosition(float x, float y) {
    transform_->positionX_ = x;
    transform_->positionY_ = y;
}