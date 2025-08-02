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
    
    // 1. SoundManager로부터 Sound 자원을 얻음
    std::filesystem::path soundPath = std::filesystem::path(SOUND_EFFECT_ASSET_ROOT_PATH) / "hit01.flac";
    std::shared_ptr<Sound> hitSound = soundManager.getSound(soundPath);

    // 2. Sound 자원을 사용하여 SoundComponent를 생성
    if (hitSound) {
        sound_ = &addComponent<SoundComponent>(soundManager, hitSound);
        // 3. 컴포넌트의 속성을 설정
        sound_->setSpatialized(true);
        sound_->setAttenuation(true);
        sound_->setRolloffFactor(1.0f);
        sound_->setReferenceDistance(5.0f);
    }

    eventListener_ = &addComponent<EventListenerComponent>(eventManager);
    eventListener_->addListener<KeyPressedEvent>([this](const KeyPressedEvent& event) { this->onPressEvent(event); });
    eventListener_->addListener<KeysHeldEvent>([this](const KeysHeldEvent& event) { this->onKeysHeldEvent(event); });
}

void TestObject::update(float deltaTime) {
    if (sound_) {
        sound_->setPosition(transform_->positionX_, transform_->positionY_, 0.0f);
    }
}

void TestObject::render() {
    render_->render();
}

void TestObject::onPressEvent(const KeyPressedEvent& event) {
    if (event.keyCode == SDL_SCANCODE_H && sound_) {
        std::cout << "'h' key pressed. Playing hit sound." << std::endl;
        // 기존에 설정된 속성으로 그냥 재생만 요청 (Fire-and-Forget)
        sound_->play();
    }
}

void TestObject::onKeysHeldEvent(const KeysHeldEvent& event) {
    float moveSpeed = 5.0f;
    for (const auto& keyInfo : event.heldKeys) {
        switch (keyInfo.scancode) {
            case SDL_SCANCODE_W:
                transform_->positionY_ -= moveSpeed;
                break;
            case SDL_SCANCODE_A:
                transform_->positionX_ -= moveSpeed;
                break;
            case SDL_SCANCODE_S:
                transform_->positionY_ += moveSpeed;
                break;
            case SDL_SCANCODE_D:
                transform_->positionX_ += moveSpeed;
                break;
            default:
                break;
        }
    }
    // 위치 업데이트는 이제 update() 함수에서 처리
}