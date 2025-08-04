#include "SoundComponent.h"

/*
 * SoundComponent의 생성자임.
 * SoundManager와 사용할 Sound 자원을 받아 초기화함.
 */
SoundComponent::SoundComponent(SoundManager& soundManager, std::shared_ptr<Sound> sound)
    : soundManager_(soundManager), sound_(std::move(sound)) {}

SoundComponent::SoundComponent(SoundComponent&& other) noexcept
    : soundManager_(other.soundManager_),
      sound_(std::move(other.sound_)),
      x_(other.x_), y_(other.y_), z_(other.z_),
      volume_(other.volume_),
      pitch_(other.pitch_),
      loop_(other.loop_),
      priority_(other.priority_),
      spatialized_(other.spatialized_),
      attenuation_(other.attenuation_),
      splitChannels_(other.splitChannels_),
      rolloffFactor_(other.rolloffFactor_),
      referenceDistance_(other.referenceDistance_),
      maxDistance_(other.maxDistance_) {}

SoundComponent& SoundComponent::operator=(SoundComponent&& other) noexcept {
    if (this != &other) {
        sound_ = std::move(other.sound_);
        x_ = other.x_;
        y_ = other.y_;
        z_ = other.z_;
        volume_ = other.volume_;
        pitch_ = other.pitch_;
        loop_ = other.loop_;
        priority_ = other.priority_;
        spatialized_ = other.spatialized_;
        attenuation_ = other.attenuation_;
        splitChannels_ = other.splitChannels_;
        rolloffFactor_ = other.rolloffFactor_;
        referenceDistance_ = other.referenceDistance_;
        maxDistance_ = other.maxDistance_;
        // soundManager_는 참조이므로 이동할 수 없음 (그대로 둠)
    }
    return *this;
}

/*
 * 사운드를 현재 설정된 속성으로 즉시 재생함 (Fire-and-Forget).
 * 모든 설정값을 SoundManager에 전달하여 재생을 위임함.
 */
void SoundComponent::play() {
    if (!sound_) {
        return; // 참조하는 사운드 자원이 없음
    }

    soundManager_.playSound(sound_.get(), {x_, y_, z_}, priority_, volume_, pitch_, 
                            loop_, spatialized_, attenuation_, 
                            rolloffFactor_, referenceDistance_, maxDistance_);
}
