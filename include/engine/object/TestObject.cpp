#include "TestObject.h"
#include <iostream>

TestObject::TestObject(EventManager& eventManager, TextureManager& textureManager, RenderManager& renderManager, SoundManager& soundManager)
{
    transform_ = &addComponent<TransformComponent>();
    transform_->positionX = 100.0f;
    transform_->positionY = 100.0f;

    std::string texturePath = static_cast<std::string>(IMAGE_ASSET_ROOT_PATH) + "example_png.png";
    if (textureManager.loadTexture(texturePath)) {
        render_ = &addComponent<RenderComponent>(renderManager, *textureManager.getTexture(texturePath));
    }

    std::string soundPath = static_cast<std::string>(SOUND_EFFECT_ASSET_ROOT_PATH) + "hit01.flac";
    sound_ = &addComponent<SoundComponent>(soundManager, soundPath);

    eventListener_ = &addComponent<EventListenerComponent>(eventManager);
    eventListener_->addListener<KeyPressedEvent>([this](const KeyPressedEvent& event) { this->onPressEvent(event); });
    eventListener_->addListener<KeysHeldEvent>([this](const KeysHeldEvent& event) { this->onKeysHeldEvent(event); });
}

void TestObject::update(float dt) {
    // Component updates are handled by their respective systems, so this can be left empty for now.
}

void TestObject::onPressEvent(const KeyPressedEvent& event) {
    if (event.keyCode == SDL_SCANCODE_H) {
        std::cout << "'h' key pressed. Playing hit sound." << std::endl;
        sound_->play(SoundPriority::HIGH);
    }
}

void TestObject::onKeysHeldEvent(const KeysHeldEvent& event) {
    float moveSpeed = 5.0f;
    for (const auto& keyInfo : event.heldKeys) {
        switch (keyInfo.scancode) {
            case SDL_SCANCODE_W:
                transform_->positionY -= moveSpeed;
                break;
            case SDL_SCANCODE_S:
                transform_->positionY += moveSpeed;
                break;
            case SDL_SCANCODE_A:
                transform_->positionX -= moveSpeed;
                break;
            case SDL_SCANCODE_D:
                transform_->positionX += moveSpeed;
                break;
            default:
                break;
        }
    }
    sound_->setPosition(transform_->positionX, transform_->positionY, 0.0f);
}
