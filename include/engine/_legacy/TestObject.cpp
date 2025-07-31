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
    std::string texturePath = static_cast<std::string>(IMAGE_ASSET_ROOT_PATH) + "example_png.png";

    if (textureManager_.loadTexture(texturePath)) {
        texture_ = textureManager_.getTexture(texturePath);
    } else {
        SDL_Log("Failed to load texture for BlankObject.");
    }

    // 사운드 컴포넌트 초기화
    std::string soundPath = static_cast<std::string>(SOUND_EFFECT_ASSET_ROOT_PATH) + "hit01.flac";
    hitSound_ = std::make_unique<SoundComponent>(soundManager_, soundPath);
}

void TestObject::onPressEvent(const KeyPressedEvent& event) {

    if (event.keyCode == SDL_SCANCODE_H) {
        std::cout << "'h' key pressed. Playing hit sound." << std::endl;
        hitSound_->play(SoundPriority::HIGH);
    }
}

void TestObject::onReleaseEvent(const KeyReleasedEvent& event) {
    const char* keyName = SDL_GetScancodeName(static_cast<SDL_Scancode>(event.keyCode));
    //std::cout << "Key Released: " << keyName << " (Scancode: " << event.keyCode << ")" << std::endl;

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

    // 3D 사운드 위치 업데이트
    // 화면 좌표를 OpenAL의 3D 공간으로 변환합니다.
    // 리스너는 (0,0,0)에 고정되어 있다고 가정합니다.
    // 화면 중앙 (windowWidth/2, windowHeight/2)을 OpenAL의 (0,0)으로 매핑합니다.
    // 픽셀 단위를 OpenAL의 단위로 스케일링합니다.
    // Z축은 깊이감을 위해 고정하거나, 필요에 따라 조절할 수 있습니다.
    
    // 윈도우 크기를 가져와서 중앙을 계산합니다.
    int windowWidth, windowHeight;
    SDL_GetWindowSize(renderManager_.getWindow(), &windowWidth, &windowHeight);

    float centerX = windowWidth / 2.0f;
    float centerY = windowHeight / 2.0f;

    // 객체의 화면 좌표를 중앙 기준으로 상대 좌표로 변환
    float relativeX = x_ - centerX;
    float relativeY = y_ - centerY;

    // OpenAL 공간으로 스케일링 (조정 가능한 값)
    // 이 스케일링 팩터가 중요합니다. 너무 작으면 효과가 미미하고, 너무 크면 사운드가 빨리 사라집니다.
    // 100픽셀이 OpenAL 1단위가 되도록 설정 (예시)
    const float pixelsPerOpenALUnit = 5.0f; // 20 픽셀이 OpenAL 1단위가 되도록 설정 (더 극적인 효과를 위해)

    // 디버그 출력: 변환된 3D 사운드 위치 확인
    float openALX = relativeX / pixelsPerOpenALUnit;
    float openALY = relativeY / pixelsPerOpenALUnit;
    float openALZ = 0.0f; // 2D 게임이므로 Z는 0으로 고정하거나, 필요에 따라 작은 값으로 설정
    std::cout << "[3D Sound Debug] TestObject Screen Pos: (" << x_ << ", " << y_ << ") -> OpenAL Pos: (" << openALX << ", " << openALY << ", " << openALZ << ")" << std::endl;

    hitSound_->setPosition(openALX, openALY, openALZ);

    // 디버그 출력: 변환된 3D 사운드 위치 확인
    std::cout << "TestObject Screen Pos: (" << x_ << ", " << y_ << ") -> OpenAL Pos: (" << openALX << ", " << openALY << ", " << openALZ << ")" << std::endl;

    
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
    // 텍스처 로딩은 생성자에서 이미 처리됨
}

void TestObject::render() {
    if (texture_) {
        renderManager_.renderTexture(texture_, x_, y_);
    }
}