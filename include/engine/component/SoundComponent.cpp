#include "SoundComponent.h"

/*
 * @brief SoundComponent의 생성자.
 * @param manager 사운드 관리를 위한 SoundManager의 참조.
 * @param path 재생할 사운드 파일의 경로.
 */
SoundComponent::SoundComponent(SoundManager& manager, const std::string& path)
    : soundManager(manager), soundPath(path), sourceIdLeft(0), sourceIdRight(0) {
    soundManager.loadSound(soundPath);

    // 3D 사운드 파라미터 기본값 설정
    // 이 값들은 OpenAL의 거리 모델(AL_INVERSE_DISTANCE_CLAMPED)에 따라 소리 감쇠에 영향을 줍니다.
    // AL_REFERENCE_DISTANCE: 이 거리에서 소리의 볼륨이 최대가 됩니다. 이 거리보다 가까워도 볼륨은 더 커지지 않습니다.
    // AL_MAX_DISTANCE: 이 거리 이후로는 소리가 더 이상 감쇠하지 않습니다. (AL_INVERSE_DISTANCE_CLAMPED 모델에서)
    // AL_ROLLOFF_FACTOR: 소리 감쇠율을 조절합니다. 값이 클수록 소리가 더 빨리 작아집니다.
    // 이 값들은 SoundManager에서 소스를 생성할 때 기본적으로 설정되지만,
    // SoundComponent에서 개별적으로 재정의할 수 있도록 합니다.
    // 여기서는 TestObject의 움직임에 따라 공간감을 더 잘 느끼도록 초기값을 설정합니다.
    // sourceIdLeft와 sourceIdRight는 play() 호출 시 SoundManager에서 할당되므로, 여기서는 설정하지 않습니다.
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
void SoundComponent::play(SoundPriority priority, float volume, float pitch, bool loop) {
    if (isPlaying()) {
        stop(); // 이미 재생 중이면 정지하고 다시 재생
    }
    // playSound는 이제 왼쪽 채널의 sourceId를 반환합니다.
    sourceIdLeft = soundManager.playSound(soundPath, priority, volume, pitch, loop);
    if (sourceIdLeft != 0) {
        // 스테레오 사운드인 경우 오른쪽 채널의 소스 ID를 가져옴
        sourceIdRight = soundManager.getSourceIdRight(sourceIdLeft);
        // 재생에 성공했다면, 저장된 위치를 즉시 적용
        soundManager.setSourcePosition(sourceIdLeft, x_, y_, z_);
    }
}

/*
 * @brief 사운드 재생을 정지함.
 */
void SoundComponent::stop() {
    if (sourceIdLeft != 0) {
        soundManager.stopSound(sourceIdLeft);
        sourceIdLeft = 0;
        sourceIdRight = 0; // 오른쪽 채널 소스도 함께 초기화
    }
}

void SoundComponent::pause() {
    if (sourceIdLeft != 0) {
        soundManager.pauseSound(sourceIdLeft);
        if (sourceIdRight != 0) {
            soundManager.pauseSound(sourceIdRight);
        }
    }
}

void SoundComponent::resume() {
    if (sourceIdLeft != 0) {
        soundManager.resumeSound(sourceIdLeft);
        if (sourceIdRight != 0) {
            soundManager.resumeSound(sourceIdRight);
        }
    }
}

void SoundComponent::togglePause() {
    if (sourceIdLeft != 0) {
        soundManager.togglePauseSound(sourceIdLeft);
        if (sourceIdRight != 0) {
            soundManager.togglePauseSound(sourceIdRight);
        }
    }
}

/*
 * @brief 사운드의 볼륨을 조절함.
 * @param volume 설정할 볼륨 값 (0.0f ~ 1.0f).
 */
void SoundComponent::setVolume(float volume) {
    if (sourceIdLeft != 0) {
        soundManager.setSourceVolume(sourceIdLeft, volume);
        if (sourceIdRight != 0) {
            soundManager.setSourceVolume(sourceIdRight, volume);
        }
    }
}

/*
 * @brief 사운드의 피치를 조절함.
 * @param pitch 설정할 피치 값 (0.5f ~ 2.0f).
 */
void SoundComponent::setPitch(float pitch) {
    if (sourceIdLeft != 0) {
        soundManager.setSourcePitch(sourceIdLeft, pitch);
        if (sourceIdRight != 0) {
            soundManager.setSourcePitch(sourceIdRight, pitch);
        }
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
    if (sourceIdLeft != 0) {
        soundManager.setSourcePosition(sourceIdLeft, x_, y_, z_);
        // SoundManager::setSourcePosition에서 스테레오 분리 로직을 처리하므로,
        // sourceIdRight는 자동으로 처리됩니다.
    }
}

/*
 * @brief 현재 사운드가 재생 중인지 확인함.
 * @return 재생 중이면 true, 아니면 false.
 */
bool SoundComponent::isPlaying() const {
    if (sourceIdLeft == 0) {
        return false;
    }
    ALint state;
    alGetSourcei(sourceIdLeft, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}
