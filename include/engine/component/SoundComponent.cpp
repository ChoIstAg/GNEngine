#include "SoundComponent.h"

/*
 * @brief SoundComponent의 생성자.
 * @param manager 사운드 관리를 위한 SoundManager의 참조.
 * @param path 재생할 사운드 파일의 경로.
 */
SoundComponent::SoundComponent(SoundManager& soundManager, const std::filesystem::path& path)
    : soundManager_(soundManager), soundPath_(path), currentSourceId_(0) {
    soundManager_.loadSound(soundPath_);
}

/*
 * @brief SoundComponent의 소멸자.
 * 재생 중인 사운드가 있다면 정지시킴.
 */
SoundComponent::~SoundComponent() {
    if (isPlaying()) {
        stop();
    }
}

/*
 * @brief 사운드를 재생함.
 * 재생 시 컴포넌트에 저장된 3D 위치를 소스에 적용함.
 * @param priority 사운드 우선순위.
 * @param volume 볼륨 (0.0f ~ 1.0f).
 * @param pitch 피치 (0.xxf ~ 2.0f).
 * @param loop 반복 재생 여부.
 */
void SoundComponent::play(SoundPriority priority, float volume, float pitch, bool loop, bool spatialized) {
    if (isPlaying()) {
        stop(); // 이미 재생 중이면 정지하고 다시 재생
    }
    // playSound는 이제 왼쪽 채널의 sourceId를 반환합니다.
    currentSourceId_ = soundManager_.playSound(soundPath_, priority, volume, pitch, loop);
    if (currentSourceId_ != 0) {
        if (spatialized) {
            // 공간화가 필요한 경우, 저장된 위치를 적용
            soundManager_.setSourcePosition(currentSourceId_, x_, y_, z_);
            alSourcei(currentSourceId_, AL_SOURCE_RELATIVE, AL_FALSE); // 공간화 활성화
        } else {
            // 공간화가 필요 없는 경우, 소스를 리스너에 상대적으로 고정하고 위치를 (0,0,0)으로 설정
            alSourcei(currentSourceId_, AL_SOURCE_RELATIVE, AL_TRUE); // 공간화 비활성화
            alSource3f(currentSourceId_, AL_POSITION, 0.0f, 0.0f, 0.0f); // 리스너에 고정
        }
    }
}

/*
 * @brief 사운드 재생을 정지함.
 */
void SoundComponent::stop() {
    if (currentSourceId_ != 0) {
        soundManager_.stopSound(currentSourceId_);
        currentSourceId_ = 0;
    }
}

void SoundComponent::pause() {
    if (currentSourceId_ != 0) {
        soundManager_.pauseSound(currentSourceId_);
    }
}

void SoundComponent::resume() {
    if (currentSourceId_ != 0) {
        soundManager_.resumeSound(currentSourceId_);
    }
}

void SoundComponent::togglePause() {
    if (currentSourceId_ != 0) {
        soundManager_.togglePauseSound(currentSourceId_);
    }
}

/*
 * @brief 사운드의 볼륨을 조절함.
 * @param volume 설정할 볼륨 값 (0.0f ~ 1.0f).
 */
void SoundComponent::setVolume(float volume) {
    if (currentSourceId_ != 0) {
        soundManager_.setSourceVolume(currentSourceId_, volume);
    }
}

/*
 * @brief 사운드의 피치를 조절함.
 * @param pitch 설정할 피치 값 (0.xxf ~ 10.0f).
 */
void SoundComponent::setPitch(float pitch) {
    if (currentSourceId_ != 0) {
        soundManager_.setSourcePitch(currentSourceId_, pitch);
    }
}

/*
 * @brief 컴포넌트의 3D 사운드 위치를 설정함.
 * 이 위치는 play()가 호출될 때 적용됨.
 * @param x, y, z 사운드 소스의 3D 좌표.
 */
void SoundComponent::setPosition(float x, float y, float z) {
    x_ = x;
    y_ = y;
    z_ = z;
    // 만약 이미 재생 중이라면, 위치를 즉시 업데이트
    if (currentSourceId_ != 0) {
        soundManager_.setSourcePosition(currentSourceId_, x_, y_, z_);
    }
}

/*
 * @brief 현재 사운드가 재생 중인지 확인함.
 * @return 재생 중이면 true, 아니면 false.
 */
bool SoundComponent::isPlaying() const {
    if (currentSourceId_ == 0) {
        return false;
    }
    ALint state;
    alGetSourcei(currentSourceId_, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

// 3D 감쇠 효과 조절 함수 구현
void SoundComponent::setRolloffFactor(float factor) {
    if (currentSourceId_ != 0) {
        soundManager_.setSourceRolloffFactor(currentSourceId_, factor);
    }
}

void SoundComponent::setReferenceDistance(float distance) {
    if (currentSourceId_ != 0) {
        soundManager_.setSourceReferenceDistance(currentSourceId_, distance);
    }
}

void SoundComponent::setMaxDistance(float distance) {
    if (currentSourceId_ != 0) {
        soundManager_.setSourceMaxDistance(currentSourceId_, distance);
    }
}
