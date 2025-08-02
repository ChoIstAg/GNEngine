#pragma once

#include <AL/al.h>

/*
 * 로드된 사운드 데이터 자체를 나타내는 자원 클래스임.
 * OpenAL 버퍼 ID와 스테레오 여부 등의 정보를 담고 있음.
 * SoundManager에 의해 관리되며, SoundComponent에서 이 객체를 참조하여 사용함.
 */
class Sound {
public:
    /*
     * Sound 객체를 생성하고 멤버 변수를 초기화함.
     */
    Sound(ALuint monoBuffer, ALuint stereoBufferRight, bool isStereo);

    /*
     * 소멸자에서 할당된 OpenAL 버퍼를 정리함.
     */
    ~Sound();

    // 복사 및 이동을 금지하여 SoundManager를 통한 공유만 허용함.
    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;
    Sound(Sound&&) = delete;
    Sound& operator=(Sound&&) = delete;

    ALuint getMonoBuffer() const { return monoBuffer_; }
    ALuint getStereoBufferRight() const { return stereoBufferRight_; }
    bool isStereo() const { return isStereo_; }

private:
    ALuint monoBuffer_;
    ALuint stereoBufferRight_;
    bool isStereo_ = false;
};
