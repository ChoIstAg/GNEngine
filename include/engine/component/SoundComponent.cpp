#include "SoundComponent.h"

/*
 * SoundComponent의 생성자임.
 * SoundManager와 사용할 Sound 자원을 받아 초기화함.
 */
SoundComponent::SoundComponent(SoundManager& soundManager, std::shared_ptr<Sound> sound)
    : soundManager_(soundManager), sound_(std::move(sound)) {}

/*
 * 사운드를 현재 설정된 속성으로 즉시 재생함 (Fire-and-Forget).
 * 모든 설정값을 SoundManager에 전달하여 재생을 위임함.
 */
void SoundComponent::play() {
    if (!sound_) {
        return; // 참조하는 사운드 자원이 없음
    }

    soundManager_.playSound(sound_.get(), 
                            priority_, 
                            volume_, 
                            pitch_, 
                            loop_, 
                            spatialized_, 
                            attenuation_, 
                            rolloffFactor_, 
                            referenceDistance_, 
                            maxDistance_);
}
