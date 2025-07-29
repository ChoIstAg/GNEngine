#include "TestObject.h"
#include <iostream>
#include <SDL3/SDL.h>

TestObject::TestObject(EventManager& eventManager, TextureManager& textureManager, RenderManager& renderManager, SoundManager& soundManager)
    : eventListener_(eventManager), textureManager_(textureManager), renderManager_(renderManager), soundManager_(soundManager), texture_(nullptr)
{
    eventListener_.addListener<KeyPressedEvent>([this](const KeyPressedEvent& event) { this->onPressEvent(event); });
    eventListener_.addListener<KeyReleasedEvent>([this](const KeyReleasedEvent& event) { this->onReleaseEvent(event); });
    eventListener_.addListener<KeysHeldEvent>([this](const KeysHeldEvent& event) { this->onKeysHeldEvent(event); });
  
    // 텍스처 로딩
    if (textureManager_.loadTexture("example_png.png")) {
        texture_ = textureManager_.getTexture("example_png.png");
    } else {
        SDL_Log("Failed to load texture for BlankObject.");
    }
}

void TestObject::onPressEvent(const KeyPressedEvent& event) {
    const char* keyName = SDL_GetScancodeName(static_cast<SDL_Scancode>(event.keyCode));
    //std::cout << "Key Pressed: " << keyName << " (Scancode: " << event.keyCode << ")" << std::endl;
}

void TestObject::onReleaseEvent(const KeyReleasedEvent& event) {
    const char* keyName = SDL_GetScancodeName(static_cast<SDL_Scancode>(event.keyCode));
    //std::cout << "Key Released: " << keyName << " (Scancode: " << event.keyCode << ")" << std::endl;

    if(event.keyCode == SDLK_L) {
        //soundManager_.setListenerPosition(0.0f, 40.0f, 0.0f);
        soundManager_.setListenerOrientation(100.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        std::cerr << "Listener orientation set to (100.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f)" << std::endl;
    }
}

void TestObject::onKeysHeldEvent(const KeysHeldEvent& event) {
    if (event.heldKeys.empty()) return;

    // 키 입력에 따라 오브젝트 이동
    float moveSpeed = 5.0f; // 픽셀/프레임
    for (const auto& keyInfo : event.heldKeys) {
        switch (keyInfo.scancode) {
            case SDL_SCANCODE_W:
                y_ -= moveSpeed;
                break;
            case SDL_SCANCODE_S:
                y_ += moveSpeed;
                break;
            case SDL_SCANCODE_A:
                x_ -= moveSpeed;
                break;
            case SDL_SCANCODE_D:
                x_ += moveSpeed;
                break;
            default:
                break;
        }
    }

    // 키 입력 테스트
    // std::cout << "Keys Held: ";
    // for (size_t i = 0; i < event.heldKeys.size(); ++i) {
    //     const auto& keyInfo = event.heldKeys[i];
    //     const char* keyName = SDL_GetScancodeName(keyInfo.scancode);
    //     std::cout << keyName << " (" << keyInfo.durationMs << "ms)" << (i == event.heldKeys.size() - 1 ? "" : ", ");
    // }
    // std::cout << std::endl;
}

void TestObject::update() {
    if (texture_) {
        renderManager_.renderTexture(texture_, x_, y_);
        //std::cout << "BlankObject position: " << x_ << ", " << y_ << std::endl; 위치 확인
    }
}