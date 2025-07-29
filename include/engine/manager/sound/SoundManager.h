#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>


// 매크로를 사용하여 오류 체크를 간소화
#define AL_CHECK_ERROR() checkAlErrors(__FILE__, __LINE__)

/*
 * @brief 사운드를 로드하고 재생 및 관리하는 클래스.
 * OpenAL을 사용하여 2D/3D 사운드를 처리함.
*/
class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    // OpenAL 초기화
    bool initAL();
    // OpenAL 종료
    void quitAL();

    /*
     * @brief 사운드 파일을 로드하고 filePath로 map에 등록함.
     * @param filePath 사운드 파일 경로.
     * @return 성공 시 true, 실패 시 false.
    */
    bool loadSound(const std::string& filePath);

    /*
     * @brief 등록된 사운드를 재생함.
     * @param filePath 재생할 사운드의 파일 경로.
     * @param volume 볼륨 (0.0f ~ 1.0f).
     * @param pitch 피치 (0.xxf ~ 2.0f).
     * @param loop 반복 재생 여부.
     * @return 재생 중인 사운드의 OpenAL 소스 ID. 실패 시 0 반환.
    */
    ALuint playSound(const std::string& filePath, float volume = 1.0f, float pitch = 1.0f, bool loop = false);

    /*
     * @brief 특정 소스 ID의 사운드를 중지함.
     * @param sourceId 중지할 사운드의 OpenAL 소스 ID.
    */
    void stopSound(ALuint sourceId);

    /*
     * @brief 모든 사운드를 중지함.
    */
    void stopAllSounds();

    /*
     * @brief 리스너(청취자)의 위치를 설정함.
     * @param x, y, z 리스너의 3D 좌표.
    */
    void setListenerPosition(float x, float y, float z);

    /*
     * @brief 리스너의 방향을 설정함.
     * @param atX, atY, atZ 리스너가 바라보는 방향 벡터.
     * @param upX, upY, upY 리스너의 상향 벡터.
    */
    void setListenerOrientation(float atX, float atY, float atZ, float upX, float upY, float upZ);

    /*
     * @brief 특정 소스 ID의 위치를 설정함 (3D 사운드).
     * @param sourceId 소스 ID.
     * @param x, y, z 소스의 3D 좌표.
    */
    void setSourcePosition(ALuint sourceId, float x, float y, float z);

    /*
     * @brief 특정 소스 ID의 볼륨을 설정함.
     * @param sourceId 소스 ID.
     * @param volume 볼륨 (0.0f ~ 1.0f).
    */
    void setSourceVolume(ALuint sourceId, float volume);

    /*
     * @brief 특정 소스 ID의 피치를 설정함.
     * @param sourceId 소스 ID.
     * @param pitch 피치 (0.5f ~ 2.0f).
    */
    void setSourcePitch(ALuint sourceId, float pitch);

private:
    ALCdevice* device_ = nullptr;
    ALCcontext* context_ = nullptr;

    // 로드된 사운드 버퍼들을 관리 (filePath -> ALuint 버퍼 ID)
    std::unordered_map<std::string, ALuint> soundBuffers_;

    // 사용 가능한 소스(스피커)들을 관리 (소스 풀링)
    std::vector<ALuint> availableSources_;
    // 재생 중인 소스들을 관리 (소스 Id -> 사운드 ID)
    std::unordered_map<ALuint, std::string> playingSources_;

    // 소스 풀에서 사용 가능한 소스를 가져옴
    ALuint getFreeSource();
    // 소스를 풀로 반환
    void returnSource(ALuint source);
    // OpenAL 오류 체크
    void checkAlErrors(const std::string& filename, int line);

    // 사운드 파일 로딩 헬퍼 함수 (내부적으로 사용)
    bool loadWav(const std::string& filePath, ALuint buffer);
    bool loadMp3(const std::string& filePath, ALuint buffer);
    bool loadOgg(const std::string& filePath, ALuint buffer);
};
