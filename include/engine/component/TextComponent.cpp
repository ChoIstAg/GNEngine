#include "TextComponent.h"

TextComponent::TextComponent(std::unique_ptr<Text> text)
    : text_(std::move(text)), transform_(nullptr) {}

void TextComponent::render() {
    if (text_ && transform_) {
        text_->render(transform_->positionX, transform_->positionY);
    }
}

void TextComponent::setText(const std::string& newText) {
    if (text_) {
        text_->setText(newText);
    }
}

void TextComponent::setColor(SDL_Color newColor) {
    if (text_) {
        text_->setColor(newColor);
    }
}
