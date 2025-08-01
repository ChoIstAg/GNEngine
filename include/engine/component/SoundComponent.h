#pragma once

#include <filesystem>
#include <iostream>
#include "./Component.h"
#include "../manager/sound/SoundManager.h"

/*
 * @brief 사운드 재생과 관련된 컴포넌트.
 * 이 컴포넌트는 특정 게임 오브젝트에 사운드 효과나 배경 음악을 첨부하는 데 사용됨.
 */
class SoundComponent : public Component {
private:
    SoundManager& soundManager_;
    std::filesystem::path soundPath_;
    ALuint currentSourceId_ = 0;

    // 3D 사운드 위치
    float x_ = 0.0f;
    float y_ = 0.0f;
    float z_ = 0.0f;

    // 사운드 재생 속성
    bool spatialized_ = false;       // 공간 음향 사용 여부
    bool attenuation_ = false;      // 감쇠 효과 사용 여부
    bool splitChannels_ = false;    // 스테레오 채널 분리 여부
    float rolloffFactor_ = 1.0f;
    float referenceDistance_ = 1.0f;
    float maxDistance_ = 100.0f;

public:
    SoundComponent(SoundManager& soundManager, const std::filesystem::path& path);
    virtual ~SoundComponent();

    void play(SoundPriority priority = SoundPriority::NORMAL, float volume = 1.0f, float pitch = 1.0f, bool loop = false);
    void stop();
    void pause();
    void resume();
    void togglePause();

    void setVolume(float volume);
    void setPitch(float pitch);
    void setPosition(float x, float y, float z);

    // 사운드 속성 설정 함수
    void setSpatialized(bool spatialized);
    void setAttenuation(bool attenuation);
    void setSplitChannels(bool split);
    void setRolloffFactor(float factor);
    void setReferenceDistance(float distance);
    void setMaxDistance(float distance);

    bool isPlaying() const;
};