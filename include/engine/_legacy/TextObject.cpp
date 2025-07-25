#include "TextObject.h"

TextObject::TextObject(std::unique_ptr<Text> text, float x, float y)
    : text(std::move(text)), x(x), y(y) {}

TextObject::~TextObject() {}

void TextObject::render() {
    if (text) {
        text->render(x, y);
    }
}

void TextObject::setText(const std::string& newText) {
    if (text) {
        text->setText(newText);
    }
}

void TextObject::setColor(SDL_Color newColor) {
    if (text) {
        text->setColor(newColor);
    }
}

void TextObject::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
}
