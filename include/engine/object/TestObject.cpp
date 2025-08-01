#include "TestObject.h"
#include <iostream>
#include <filesystem>

TestObject::TestObject(EventManager& eventManager, TextureManager& textureManager, RenderManager& renderManager, SoundManager& soundManager)
{
    transform_ = &addComponent<TransformComponent>(100.0f, 100.0f);

    std::filesystem::path texturePath = std::filesystem::path(IMAGE_ASSET_ROOT_PATH) / "example_png.png";
    if (textureManager.loadTexture(texturePath)) {
        render_ = &addComponent<RenderComponent>(renderManager, *textureManager.getTexture(texturePath), *transform_);
    }
    
    std::filesystem::path soundPath = std::filesystem::path(SOUND_EFFECT_ASSET_ROOT_PATH) / "hit01.flac";
    sound_ = &addComponent<SoundComponent>(soundManager, soundPath);
    
    /* 감쇠 효과 조절 */
    sound_->setRolloffFactor(0.5f); // 감쇠 속도를 절반으로 줄임 (소리가 더 멀리까지 들림)
    sound_->setReferenceDistance(5.0f); // 5.0 단위 거리까지 최대 볼륨 유지

    eventListener_ = &addComponent<EventListenerComponent>(eventManager);
    eventListener_->addListener<KeyPressedEvent>([this](const KeyPressedEvent& event) { this->onPressEvent(event); });
    eventListener_->addListener<KeysHeldEvent>([this](const KeysHeldEvent& event) { this->onKeysHeldEvent(event); });
}

void TestObject::update(float dt) {
}

void TestObject::render() {
    render_->render();
}

void TestObject::onPressEvent(const KeyPressedEvent& event) {
    if (event.keyCode == SDL_SCANCODE_H) {
        std::cout << "'h' key pressed. Playing hit sound." << std::endl;
        // sound_->setSpatialized(false); // 3D 공간 음향 비활성화
        // sound_->setSplitChannels(false); // 스테레오 채널을 모노로 처리
        // sound_->setAttenuation(false); // 거리 감쇠 비활성화
        sound_->play(SoundPriority::HIGH, 1.0f, 1.0f, false);
    }
}

void TestObject::onKeysHeldEvent(const KeysHeldEvent& event) {
    float moveSpeed = 5.0f;
    for (const auto& keyInfo : event.heldKeys) {
        switch (keyInfo.scancode) {
            case SDL_SCANCODE_W:
                transform_->positionY_ -= moveSpeed;
                break;
            case SDL_SCANCODE_S:
                transform_->positionY_ += moveSpeed;
                break;
            case SDL_SCANCODE_A:
                transform_->positionX_ -= moveSpeed;
                break;
            case SDL_SCANCODE_D:
                transform_->positionX_ += moveSpeed;
                break;
            default:
                break;
        }
    }
    sound_->setPosition(transform_->positionX_, transform_->positionY_, 0.0f);
}