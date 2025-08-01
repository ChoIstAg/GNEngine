#include "SoundComponent.h"

/* 
 * SoundComponent의 생성자임.
 * 사운드 관리를 위한 SoundManager의 참조와 재생할 사운드 파일의 경로를 받음.
 */
SoundComponent::SoundComponent(SoundManager& soundManager, const std::filesystem::path& path)
    : soundManager_(soundManager), 
      soundPath_(path), 
      currentSourceId_(0),
      spatialized_(true),
      attenuation_(true),
      splitChannels_(true),
      rolloffFactor_(1.0f),
      referenceDistance_(1.0f),
      maxDistance_(100.0f) {
    soundManager_.loadSound(soundPath_);
}

/* 
 * SoundComponent의 소멸자임.
 * 재생 중인 사운드가 있다면 정지시킴.
 */
SoundComponent::~SoundComponent() {
    if (isPlaying()) {
        stop();
    }
}

/* 
 * 사운드를 재생함.
 * 컴포넌트에 저장된 속성(공간 음향, 감쇠, 채널 분리 등)을 사용하여 SoundManager에 재생을 요청함.
 * 이미 재생 중인 사운드가 있다면 정지하고 새로 재생함.
 */
void SoundComponent::play(SoundPriority priority, float volume, float pitch, bool loop) {
    if (isPlaying()) {
        stop();
    }

    currentSourceId_ = soundManager_.playSound(soundPath_, priority, volume, pitch, loop, 
                                               spatialized_, attenuation_, splitChannels_,
                                               rolloffFactor_, referenceDistance_, maxDistance_);

    if (currentSourceId_ != 0 && spatialized_) {
        soundManager_.setSourcePosition(currentSourceId_, x_, y_, z_);
    }
}

/* 
 * 사운드 재생을 정지함.
 */
void SoundComponent::stop() {
    if (currentSourceId_ != 0) {
        soundManager_.stopSound(currentSourceId_);
        currentSourceId_ = 0;
    }
}

/* 
 * 사운드 재생을 일시 정지함.
 */
void SoundComponent::pause() {
    if (currentSourceId_ != 0) {
        soundManager_.pauseSound(currentSourceId_);
    }
}

/* 
 * 일시 정지된 사운드를 다시 재생함.
 */
void SoundComponent::resume() {
    if (currentSourceId_ != 0) {
        soundManager_.resumeSound(currentSourceId_);
    }
}

/* 
 * 사운드 재생 상태를 토글함 (재생 중 -> 일시 정지, 일시 정지 -> 재생).
 */
void SoundComponent::togglePause() {
    if (currentSourceId_ != 0) {
        soundManager_.togglePauseSound(currentSourceId_);
    }
}

/* 
 * 사운드의 볼륨을 조절함.
 */
void SoundComponent::setVolume(float volume) {
    if (currentSourceId_ != 0) {
        soundManager_.setSourceVolume(currentSourceId_, volume);
    }
}

/* 
 * 사운드의 피치를 조절함.
 */
void SoundComponent::setPitch(float pitch) {
    if (currentSourceId_ != 0) {
        soundManager_.setSourcePitch(currentSourceId_, pitch);
    }
}

/* 
 * 컴포넌트의 3D 사운드 위치를 설정함.
 * 이 위치는 play()가 호출될 때 적용되며, 
 * spatialized_가 true이고 사운드가 재생 중일 때 즉시 위치를 업데이트함.
 */
void SoundComponent::setPosition(float x, float y, float z) {
    x_ = x;
    y_ = y;
    z_ = z;

    if (currentSourceId_ != 0) {
        if (spatialized_) {
            /* [DEBUG] 공간 음향이 활성화된 사운드의 위치를 업데이트함. */
            std::cout << "[DEBUG] Updating position for spatialized sound. Source ID: " << currentSourceId_ << std::endl;
            soundManager_.setSourcePosition(currentSourceId_, x_, y_, z_);
        } else {
            /* [DEBUG] 공간 음향이 비활성화된 사운드의 위치 업데이트를 건너뜀. */
            std::cout << "[DEBUG] Skipping position update for non-spatialized sound. Source ID: " << currentSourceId_ << std::endl;
        }
    }
}

/* 
 * 현재 사운드가 재생 중인지 확인함.
 */
bool SoundComponent::isPlaying() const {
    if (currentSourceId_ == 0) {
        return false;
    }
    ALint state;
    alGetSourcei(currentSourceId_, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

/* 
 * 공간 음향 사용 여부를 설정함.
 */
void SoundComponent::setSpatialized(bool spatialized) {
    spatialized_ = spatialized;
}

/* 
 * 거리 감쇠 효과 사용 여부를 설정함.
 */
void SoundComponent::setAttenuation(bool attenuation) {
    attenuation_ = attenuation;
}

/* 
 * 스테레오 채널 분리 여부를 설정함.
 */
void SoundComponent::setSplitChannels(bool split) {
    splitChannels_ = split;
}

/* 
 * 감쇠율(Rolloff Factor)을 설정함.
 * 재생 중에 호출되면 즉시 적용됨.
 */
void SoundComponent::setRolloffFactor(float factor) {
    rolloffFactor_ = factor;
    if (currentSourceId_ != 0) {
        soundManager_.setSourceRolloffFactor(currentSourceId_, rolloffFactor_);
    }
}

/* 
 * 기준 거리(Reference Distance)를 설정함.
 * 재생 중에 호출되면 즉시 적용됨.
 */
void SoundComponent::setReferenceDistance(float distance) {
    referenceDistance_ = distance;
    if (currentSourceId_ != 0) {
        soundManager_.setSourceReferenceDistance(currentSourceId_, referenceDistance_);
    }
}

/* 
 * 최대 거리(Max Distance)를 설정함.
 * 재생 중에 호출되면 즉시 적용됨.
 */
void SoundComponent::setMaxDistance(float distance) {
    maxDistance_ = distance;
    if (currentSourceId_ != 0) {
        soundManager_.setSourceMaxDistance(currentSourceId_, maxDistance_);
    }
}
