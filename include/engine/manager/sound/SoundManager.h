#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional> // For std::hash<std::filesystem::path>

// 매크로를 사용하여 오류 체크를 간소화
#define AL_CHECK_ERROR() checkAlErrors(__FILE__, __LINE__)

/*
 * @brief 사운드 재생 우선순위.
 * @note 모든 소스가 사용중일 때 우선순위가 높은 것이 추가될 경우 우선순위가 가장 낮은 것의 재생을 중지함.
 */
enum class SoundPriority {
    LOW, /* 0 : (예: 중요하지 않은 멀리서 들리는 효과음) */
    NORMAL, /* 1 (예: 일반 효과음)*/
    HIGH, /* 2 (예: 피격음, 중요한 효과음)*/
    CRITICAL /* 3 (예: 메인 배경음악, UI 효과음 등 매우 중요한 효과음.)*/
};

/*
 * @brief 사운드를 로드하고 재생 및 관리하는 클래스.
 * 보이스 관리(고정된 수의 소스 풀, 우선순위 기반 보이스 훔치기) 기능을 포함함.
*/
class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    bool initAL();
    void quitAL();

    bool loadSound(const std::filesystem::path& filePath);

    /*
     * @brief 등록된 사운드를 재생함.
     * @param filePath 재생할 사운드의 파일 경로.
     * @param priority 사운드 우선순위.
     * @param volume 볼륨 (0.0f ~ 1.0f).
     * @param pitch 피치 (0.xxf ~ 2.0f).
     * @param loop 반복 재생 여부.
     * @return 재생에 성공한 경우 OpenAL 소스 ID, 실패 시 0 반환.
    */
    ALuint playSound(const std::filesystem::path& filePath, 
                     SoundPriority priority = SoundPriority::NORMAL, 
                     float volume = 1.0f, 
                     float pitch = 1.0f, 
                     bool loop = false, 
                     bool spatialized = true, 
                     bool attenuation = true, 
                     bool splitChannels = true,
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

    // 3D 사운드 파라미터 설정
    void setSourceRolloffFactor(ALuint sourceId, float factor);
    void setSourceReferenceDistance(ALuint sourceId, float distance);
    void setSourceMaxDistance(ALuint sourceId, float distance);

    // 스테레오 사운드 재생 시 오른쪽 채널의 소스 ID를 가져옴
    ALuint getSourceIdRight(ALuint sourceIdLeft);

private:
    /*
     * @brief 현재 재생 중인 소스(보이스)의 정보를 담는 구조체.
     */
    struct Voice {
        ALuint sourceIdLeft = 0; // 모노 사운드 또는 스테레오 사운드의 왼쪽 채널 소스
        ALuint sourceIdRight = 0; // 스테레오 사운드의 오른쪽 채널 소스 (스테레오 사운드인 경우에만 사용)
        std::filesystem::path soundPath;
        SoundPriority priority = SoundPriority::NORMAL;
        bool isPlaying = false;
        bool isStereo = false; // 이 보이스가 스테레오 사운드를 재생 중인지 여부
    };

    struct SoundBufferInfo {
        ALuint monoBuffer = 0; // 모노 사운드 또는 스테레오 사운드의 왼쪽 채널
        ALuint stereoBufferRight = 0; // 스테레오 사운드의 오른쪽 채널 (스테레오 사운드인 경우에만 사용)
        bool isStereo = false;
    };

    static constexpr int MAX_VOICES = 64; // 동시 재생 가능한 최대 소스 수

    ALCdevice* device_ = nullptr;
    ALCcontext* context_ = nullptr;

    std::unordered_map<std::filesystem::path, SoundBufferInfo> soundBuffers_;
    std::vector<Voice> voicePool_; // 미리 할당된 보이스(소스) 풀
    
    // 사용 가능한 보이스를 찾거나, 우선순위가 낮은 보이스를 훔침
    Voice* findAvailableVoice(SoundPriority priority, bool forStereo = false); // 스테레오 사운드용 보이스를 찾을 때 사용
    void releaseVoice(ALuint sourceId); // 보이스를 풀로 반환하는 대신 상태를 '사용 가능'으로 변경
    void releaseVoice(Voice* voice); // Voice 객체를 직접 받아 해제

    void checkAlErrors(const std::string& filename, int line);

    bool loadWav(const std::filesystem::path& filePath, SoundBufferInfo& bufferInfo);
    bool loadMp3(const std::filesystem::path& filePath, SoundBufferInfo& bufferInfo);
    bool loadFlac(const std::filesystem::path& filePath, SoundBufferInfo& bufferInfo);
    bool loadOgg(const std::filesystem::path& filePath, SoundBufferInfo& bufferInfo);
};
