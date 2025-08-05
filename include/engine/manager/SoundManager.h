#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include "engine/resource/Sound.h"

#define AL_CHECK_ERROR() checkAlErrors(__FILE__, __LINE__)

struct Position {
    float x, y, z;
};

enum class SoundPriority {
    LOW,
    NORMAL,
    HIGH,
    CRITICAL
};

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    bool initAL();
    void quitAL();

    std::shared_ptr<Sound> getSound(const std::filesystem::path& filePath);

    ALuint playSound(Sound* sound,
                     Position position = {0.0f, 0.0f, 0.0f},
                     SoundPriority priority = SoundPriority::NORMAL,
                     float volume = 1.0f,
                     float pitch = 1.0f,
                     bool loop = false,
                     bool spatialized = false,
                     bool attenuation = false,
                     float rolloffFactor = 1.0f,
                     float referenceDistance = 1.0f,
                     float maxDistance = 100.0f);

    void stopSound(ALuint sourceId);
    void pauseSound(ALuint sourceId);
    void resumeSound(ALuint sourceId);
    void togglePauseSound(ALuint sourceId);
    void stopAllSounds();

    void setListenerPosition(float x, float y, float z);
    void setListenerOrientation(float atX, float atY, float atZ, float upX, float upY, float upZ);

    void setSourcePosition(ALuint sourceId, float x, float y, float z);
    void setSourceVolume(ALuint sourceId, float volume);
    void setSourcePitch(ALuint sourceId, float pitch);

    void setSourceRolloffFactor(ALuint sourceId, float factor);
    void setSourceReferenceDistance(ALuint sourceId, float distance);
    void setSourceMaxDistance(ALuint sourceId, float distance);

    ALuint getSourceIdRight(ALuint sourceIdLeft);

private:
    struct Voice {
        ALuint sourceIdLeft = 0;
        ALuint sourceIdRight = 0;
        Sound* sound = nullptr; // 이제 Sound 객체를 직접 가리킴
        SoundPriority priority = SoundPriority::NORMAL;
        bool isPlaying = false;
        bool isSplitStereo = false;
    };

    static constexpr int MAX_VOICES = 64;

    ALCdevice* device_ = nullptr;
    ALCcontext* context_ = nullptr;

    Position listenerPosition_ = {0.0f, 0.0f, 0.0f};

    std::unordered_map<std::filesystem::path, std::shared_ptr<Sound>> soundCache_;
    std::vector<Voice> voicePool_;
    
    Voice* findAvailableVoice(SoundPriority priority, bool forStereo = false);
    void releaseVoice(Voice* voice);

    void checkAlErrors(const std::string& filename, int line);

    // 로더 함수들은 이제 Sound 객체를 생성하여 반환해야 하므로, 내부 구현에서 처리
    bool loadWav(const std::filesystem::path& filePath, ALuint& monoBuffer, ALuint& stereoBufferRight, bool& isStereo);
    bool loadMp3(const std::filesystem::path& filePath, ALuint& monoBuffer, ALuint& stereoBufferRight, bool& isStereo);
    bool loadFlac(const std::filesystem::path& filePath, ALuint& monoBuffer, ALuint& stereoBufferRight, bool& isStereo);
    bool loadOgg(const std::filesystem::path& filePath, ALuint& monoBuffer, ALuint& stereoBufferRight, bool& isStereo);
};



