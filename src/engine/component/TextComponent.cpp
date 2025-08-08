#include "engine/component/TextComponent.h"

TextComponent::TextComponent(std::unique_ptr<Text> text)
    : text_(std::move(text)) {}

