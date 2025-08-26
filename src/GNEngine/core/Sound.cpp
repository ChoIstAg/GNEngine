#include "GNEngine/core/Sound.h"

/*
 * Sound 객체를 생성하고 멤버 변수를 초기화함.
 */
Sound::Sound(ALuint monoBuffer, ALuint stereoBufferRight, bool isStereo)
    : monoBuffer_(monoBuffer),
      stereoBufferRight_(stereoBufferRight),
      isStereo_(isStereo) {}

/*
 * 소멸자에서 할당된 OpenAL 버퍼를 정리함.
 * monoBuffer_가 0이 아니면 항상 삭제를 시도함.
 * isStereo_가 true일 때만 stereoBufferRight_의 삭제를 시도함.
 */
Sound::~Sound() {
    if (monoBuffer_ != 0 && alIsBuffer(monoBuffer_)) {
        alDeleteBuffers(1, &monoBuffer_);
    }
    if (isStereo_ && stereoBufferRight_ != 0 && alIsBuffer(stereoBufferRight_)) {
        alDeleteBuffers(1, &stereoBufferRight_);
    }
}



