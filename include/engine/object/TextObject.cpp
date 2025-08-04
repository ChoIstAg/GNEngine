#include "TextObject.h"
#include "engine/component/TransformComponent.h"

TextObject::TextObject(EntityManager& entityManager, TextManager& textManager, std::unique_ptr<Text> text, float x, float y)
    : entityId_(entityManager.createEntity())
{
    entityManager.addComponent<TransformComponent>(entityId_, x, y, 1.0f, 1.0f, 0.0f);
    entityManager.addComponent<TextComponent>(entityId_, std::move(text));
}
