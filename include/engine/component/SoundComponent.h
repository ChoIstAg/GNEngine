#pragma once

#include <filesystem>
#include <memory>
#include "./Component.h"
#include "../manager/sound/SoundManager.h"
#include "../resource/sound/Sound.h"

/*
 * 사운드 재생을 위한 설정 정보를 담고, "Fire-and-Forget" 방식으로 재생을 요청하는 컴포넌트임.
 */
class SoundComponent : public Component {
public:
    SoundComponent(SoundManager& soundManager, std::shared_ptr<Sound> sound);
    virtual ~SoundComponent() = default;

    /* 현재 설정된 속성으로 사운드를 즉시 재생함 (Fire-and-Forget) */
    void play();

    // --- 재생 전 사운드 속성 설정 함수들 ---
    void setPosition(float x, float y, float z) { x_ = x; y_ = y; z_ = z; }
    void setVolume(float volume) { volume_ = volume; }
    void setPitch(float pitch) { pitch_ = pitch; }
    void setLoop(bool loop) { loop_ = loop; }
    void setPriority(SoundPriority priority) { priority_ = priority; }

    void setSpatialized(bool spatialized) { spatialized_ = spatialized; }
    void setAttenuation(bool attenuation) { attenuation_ = attenuation; }
    void setSplitChannels(bool split) { splitChannels_ = split; }
    void setRolloffFactor(float factor) { rolloffFactor_ = factor; }
    void setReferenceDistance(float distance) { referenceDistance_ = distance; }
    void setMaxDistance(float distance) { maxDistance_ = distance; }

private:
    SoundManager& soundManager_;
    std::shared_ptr<Sound> sound_;

    // --- 재생 속성 --- 
    float x_ = 0.0f, y_ = 0.0f, z_ = 0.0f;
    float volume_ = 1.0f;
    float pitch_ = 1.0f;
    bool loop_ = false;
    SoundPriority priority_ = SoundPriority::NORMAL;

    bool spatialized_ = true;
    bool attenuation_ = true;
    bool splitChannels_ = true;
    float rolloffFactor_ = 1.0f;
    float referenceDistance_ = 1.0f;
    float maxDistance_ = 100.0f;
};
