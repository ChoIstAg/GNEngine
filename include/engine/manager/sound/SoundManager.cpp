#include "SoundManager.h"
#include <iostream>
#include <algorithm> // for std::find_if
#include <filesystem>

#include <AL/alc.h>
#include <AL/alext.h> 

/* for decording sound files*/
#define DR_MP3_IMPLEMENTATION
#include "dr_libs/dr_mp3.h"
#define DR_WAV_IMPLEMENTATION
#include "dr_libs/dr_wav.h"
#define DR_FLAC_IMPLEMENTATION
#include "dr_libs/dr_flac.h"

#include "ogg/stb_vorbis.c"

/*
 * @brief SoundManager 생성자. OpenAL 초기화를 수행함.
 */
SoundManager::SoundManager() {
    initAL();
}

/*
 * @brief SoundManager 소멸자. OpenAL 리소스를 정리하고 종료함.
 */
SoundManager::~SoundManager() {
    quitAL();
}

/*
 * @brief OpenAL을 초기화하고 오디오 장치와 컨텍스트를 설정함.
 * @return 초기화 성공 시 true, 실패 시 false를 반환함.
 * @note HRTF 지원을 확인하고 가능한 경우 활성화함.
 */
bool SoundManager::initAL() {
    // 기본 오디오 장치를 염.
    device_ = alcOpenDevice(NULL);
    if (!device_) {
        std::cerr << "Failed to open OpenAL device(" << alcGetString(NULL, alcGetError(NULL)) << ")." << std::endl;
        return false;
    }
    
    // HRTF(Head-Related Transfer Function)를 활성화하기 위한 속성.
    ALCint attr[] = {
        ALC_HRTF_SOFT, ALC_TRUE,
        0
    };

    // 사용 가능한 HRTF를 쿼리하고 설정.
    const ALCchar* hrtf_specifier = alcGetString(device_, ALC_HRTF_SPECIFIER_SOFT);
    if (hrtf_specifier && *hrtf_specifier) {
        std::cout << "Available HRTF specifiers: " << hrtf_specifier << std::endl;
        // 첫 번째 HRTF를 선택하여 사용 (OpenAL Soft는 1부터 시작)
        attr[2] = ALC_HRTF_ID_SOFT;
        attr[3] = 1; 
        context_ = alcCreateContext(device_, attr);
    } else {
        std::cout << "No HRTF specifiers found. Creating context without HRTF." << std::endl;
        context_ = alcCreateContext(device_, NULL);
    }
    if (!context_) {
        std::cerr << "Failed to create OpenAL context." << std::endl;
        alcCloseDevice(device_);
        device_ = nullptr;
        return false;
    }
    
    // 생성된 컨텍스트를 현재 컨텍스트로 만듦.
    if (!alcMakeContextCurrent(context_)) {
        std::cerr << "Failed to make OpenAL context current." << std::endl;
        alcDestroyContext(context_);
        alcCloseDevice(device_);
        context_ = nullptr;
        device_ = nullptr;
        return false;
    }
    
    AL_CHECK_ERROR();

    // OpenAL 거리 모델 설정.
    // AL_INVERSE_DISTANCE: 거리에 따라 볼륨이 감쇠하는 가장 일반적인 모델.
    // AL_INVERSE_DISTANCE_CLAMPED: 거리에 따라 볼륨이 감쇠하되, 최대 거리 이상부터는 볼륨이 더이상 감소하지 않음.
    alDistanceModel(AL_INVERSE_DISTANCE);
    AL_CHECK_ERROR();

    // 리스너(청자)의 기본 위치와 방향을 설정함. (위치: 원점, 방향: -Z축, 상향: +Y축)
    setListenerPosition(0.0f, 0.0f, 0.0f);
    setListenerOrientation(0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

    // 보이스 풀(재생 채널 풀)을 초기화함.
    voicePool_.resize(MAX_VOICES);
    for (int i = 0; i < MAX_VOICES; ++i) {
        // 스테레오 분리를 위해 각 보이스마다 두 개의 소스(왼쪽, 오른쪽)를 생성함.
        alGenSources(1, &voicePool_[i].sourceIdLeft);
        AL_CHECK_ERROR();
        if (voicePool_[i].sourceIdLeft == 0) {
            std::cerr << "Failed to generate OpenAL left source." << std::endl;
            // 실패 시 이전에 생성된 소스들을 정리.
            for (int j = 0; j < i; ++j) {
                alDeleteSources(1, &voicePool_[j].sourceIdLeft);
                alDeleteSources(1, &voicePool_[j].sourceIdRight);
            }
            return false;
        }
        alGenSources(1, &voicePool_[i].sourceIdRight);
        AL_CHECK_ERROR();
        // 실패 시 이전에 생성된 소스들을 정리.
        if (voicePool_[i].sourceIdRight == 0) {
            std::cerr << "Failed to generate OpenAL right source." << std::endl;
            alDeleteSources(1, &voicePool_[i].sourceIdLeft);
            for (int j = 0; j < i; ++j) {
                alDeleteSources(1, &voicePool_[j].sourceIdLeft);
                alDeleteSources(1, &voicePool_[j].sourceIdRight);
            }
            return false;
        }

        // 각 소스의 기본 감쇠(attenuation) 관련 파라미터를 설정함.
        alSourcef(voicePool_[i].sourceIdLeft, AL_REFERENCE_DISTANCE, 1.0f); // 1.0 단위 거리에서 최대 볼륨.
        alSourcef(voicePool_[i].sourceIdLeft, AL_ROLLOFF_FACTOR, 1.0f);    // 감쇠율 (기본값 1.0).
        alSourcef(voicePool_[i].sourceIdRight, AL_REFERENCE_DISTANCE, 1.0f);
        alSourcef(voicePool_[i].sourceIdRight, AL_ROLLOFF_FACTOR, 1.0f);
        AL_CHECK_ERROR();
    }

    std::cout << "OpenAL initialized with " << MAX_VOICES << " voices." << std::endl;
    return true;
}

/*
 * @brief 사용된 모든 OpenAL 리소스를 정리하고 종료함.
 * @note 재생 중인 모든 소리를 멈추고, 소스, 버퍼, 컨텍스트, 장치를 순서대로 정리함.
 */
void SoundManager::quitAL() {
    if (context_ && device_) {
        alcMakeContextCurrent(context_);
        AL_CHECK_ERROR();

        stopAllSounds();

        // 보이스 풀의 모든 소스를 삭제함.
        for (auto& voice : voicePool_) {
            if (alIsSource(voice.sourceIdLeft)) {
                alDeleteSources(1, &voice.sourceIdLeft);
            }
            if (alIsSource(voice.sourceIdRight)) {
                alDeleteSources(1, &voice.sourceIdRight);
            }
        }
        voicePool_.clear();

        // 로드된 모든 사운드 버퍼를 삭제함.
        for (auto const& [id, bufferInfo] : soundBuffers_) {
            if (bufferInfo.monoBufferLeft != 0 && alIsBuffer(bufferInfo.monoBufferLeft)) {
                alDeleteBuffers(1, &bufferInfo.monoBufferLeft);
            }
            if (bufferInfo.monoBufferRight != 0 && alIsBuffer(bufferInfo.monoBufferRight)) {
                alDeleteBuffers(1, &bufferInfo.monoBufferRight);
            }
            if (bufferInfo.stereoBuffer != 0 && alIsBuffer(bufferInfo.stereoBuffer)) {
                alDeleteBuffers(1, &bufferInfo.stereoBuffer);
            }
        }
        soundBuffers_.clear();

        // 컨텍스트와 장치를 닫음.
        alcMakeContextCurrent(NULL);
        alcDestroyContext(context_);
        context_ = nullptr;

        alcCloseDevice(device_);
        device_ = nullptr;
    }
    std::cout << "OpenAL shut down." << std::endl;
}

/*
 * @brief OpenAL API 호출 후 발생할 수 있는 오류를 확인하고 출력함.
 * @param filename 오류가 발생한 소스 파일 이름.
 * @param line 오류가 발생한 소스 파일의 라인 번호.
 */
void SoundManager::checkAlErrors(const std::string& filename, int line) {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "OpenAL Error in " << filename << " (" << line << "): " << alGetString(error) << std::endl;
    }
}

/*
 * @brief 지정된 경로의 사운드 파일을 로드하여 OpenAL 버퍼를 생성함.
 * @param filePath 로드할 사운드 파일의 경로.
 * @return 로딩 성공 시 true, 실패 시 false를 반환함.
 * @note 파일 확장자를 기반으로 적절한 디코더(WAV, MP3, OGG, FLAC)를 호출함.
 *       이미 로드된 파일은 다시 로드하지 않음.
 */
bool SoundManager::loadSound(const std::filesystem::path& filePath) {
    if (soundBuffers_.count(filePath)) {
        return true; // 이미 로드됨.
    }

    SoundBufferInfo bufferInfo;
    
    std::string extension = filePath.extension().string();
    
    bool loaded = false;
    if (extension == ".wav" || extension == ".WAV") {
        loaded = loadWav(filePath, bufferInfo);
    } else if (extension == ".mp3" || extension == ".MP3") {
        loaded = loadMp3(filePath, bufferInfo);
    } else if (extension == ".ogg" || extension == ".OGG") {
        loaded = loadOgg(filePath, bufferInfo);
    } else if (extension == ".flac" || extension == ".FLAC") {
        loaded = loadFlac(filePath, bufferInfo);
    } else {
        std::cerr << "Unsupported sound file format: " << extension << std::endl;
        return false;
    }

    if (loaded) {
        soundBuffers_[filePath] = bufferInfo;
        return true;
    } else {
        // 로딩 실패 시 생성되었을 수 있는 버퍼를 정리함.
        if (bufferInfo.monoBufferLeft != 0) alDeleteBuffers(1, &bufferInfo.monoBufferLeft);
        if (bufferInfo.monoBufferRight != 0) alDeleteBuffers(1, &bufferInfo.monoBufferRight);
        if (bufferInfo.stereoBuffer != 0) alDeleteBuffers(1, &bufferInfo.stereoBuffer);
        return false;
    }
}

/*
 * @brief 로드된 사운드를 재생함.
 * @param filePath 재생할 사운드의 파일 경로.
 * @param priority 사운드 재생 우선순위.
 * @param volume 볼륨 (0.0 ~ 1.0).
 * @param pitch 피치 (재생 속도 조절).
 * @param loop 반복 재생 여부.
 * @param spatialized 3D 공간 음향 적용 여부. false일 경우 2D 사운드로 재생.
 * @param attenuation 거리 감쇠 적용 여부.
 * @param splitChannels 스테레오 사운드를 좌우 채널로 분리하여 3D 공간감을 향상시킬지 여부.
 * @param rolloffFactor 거리 감쇠율.
 * @param referenceDistance 최대 볼륨으로 들리는 기준 거리.
 * @param maxDistance 사운드가 더 이상 감쇠하지 않는 최대 거리.
 * @return 재생에 사용된 OpenAL 소스의 ID (왼쪽 채널). 실패 시 0을 반환.
 */
ALuint SoundManager::playSound(const std::filesystem::path& filePath, SoundPriority priority, float volume, float pitch, bool loop, bool spatialized, bool attenuation, float rolloffFactor, float referenceDistance, float maxDistance) {
    auto it = soundBuffers_.find(filePath);
    if (it == soundBuffers_.end()) {
        std::cerr << "Sound with ID '" << filePath.string() << "' not found." << std::endl;
        return 0;
    }

    const auto& bufferInfo = it->second;
    bool is3DStereo = bufferInfo.isStereo && spatialized;

    Voice* voice = findAvailableVoice(priority, is3DStereo);
    if (!voice) {
        return 0; // 사용 가능한 보이스가 없음.
    }

    voice->soundPath = filePath;
    voice->priority = priority;
    voice->isPlaying = true;
    voice->isSplitStereo = is3DStereo;

    ALuint sourceLeft = voice->sourceIdLeft;
    ALuint sourceRight = voice->sourceIdRight;

    // --- 공통 소스 설정 (왼쪽 채널 또는 주 소스) ---
    alSourcef(sourceLeft, AL_GAIN, volume);
    alSourcef(sourceLeft, AL_PITCH, pitch);
    alSourcei(sourceLeft, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    alSourcei(sourceLeft, AL_SOURCE_RELATIVE, !spatialized);
    AL_CHECK_ERROR();

    // --- 재생 방식에 따른 분기 처리 ---
    if (is3DStereo) {
        // [CASE 1: 3D 스테레오] - 2개의 소스, 2개의 모노 버퍼 사용
        alSourcei(sourceLeft, AL_BUFFER, bufferInfo.monoBufferLeft);
        alSourcei(sourceRight, AL_BUFFER, bufferInfo.monoBufferRight);

        // 오른쪽 채널 소스 설정
        alSourcef(sourceRight, AL_GAIN, volume);
        alSourcef(sourceRight, AL_PITCH, pitch);
        alSourcei(sourceRight, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcei(sourceRight, AL_SOURCE_RELATIVE, !spatialized);

        // 3D 감쇠 설정
        if (attenuation) {
            alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
            alSourcef(sourceLeft, AL_ROLLOFF_FACTOR, rolloffFactor);
            alSourcef(sourceLeft, AL_REFERENCE_DISTANCE, referenceDistance);
            alSourcef(sourceLeft, AL_MAX_DISTANCE, maxDistance);
            alSourcef(sourceRight, AL_ROLLOFF_FACTOR, rolloffFactor);
            alSourcef(sourceRight, AL_REFERENCE_DISTANCE, referenceDistance);
            alSourcef(sourceRight, AL_MAX_DISTANCE, maxDistance);
        } else {
            alDistanceModel(AL_NONE);
        }
        AL_CHECK_ERROR();

        alSourcePlay(sourceLeft);
        alSourcePlay(sourceRight);

    } else if (bufferInfo.isStereo) {
        // [CASE 2: 2D 스테레오] - 1개의 소스, 1개의 스테레오 버퍼 사용
        alSourcei(sourceLeft, AL_BUFFER, bufferInfo.stereoBuffer);
        alSource3f(sourceLeft, AL_POSITION, 0.0f, 0.0f, 0.0f); // 위치 고정
        alDistanceModel(AL_NONE); // 2D는 감쇠 없음
        AL_CHECK_ERROR();
        alSourcePlay(sourceLeft);

    } else {
        // [CASE 3: 모노] - 1개의 소스, 1개의 모노 버퍼 사용
        alSourcei(sourceLeft, AL_BUFFER, bufferInfo.monoBufferLeft);
        if (!spatialized) {
            alSource3f(sourceLeft, AL_POSITION, 0.0f, 0.0f, 0.0f); // 2D 모노 위치 고정
        }
        // 3D 감쇠 설정
        if (spatialized && attenuation) {
            alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
            alSourcef(sourceLeft, AL_ROLLOFF_FACTOR, rolloffFactor);
            alSourcef(sourceLeft, AL_REFERENCE_DISTANCE, referenceDistance);
            alSourcef(sourceLeft, AL_MAX_DISTANCE, maxDistance);
        } else {
            alDistanceModel(AL_NONE);
        }
        AL_CHECK_ERROR();
        alSourcePlay(sourceLeft);
    }

    AL_CHECK_ERROR();
    return sourceLeft;
}

/*
 * @brief 지정된 소스의 재생을 일시 정지함.
 * @param sourceId 일시 정지할 OpenAL 소스 ID.
 */
void SoundManager::pauseSound(ALuint sourceId) {
    alSourcePause(sourceId);
    AL_CHECK_ERROR();
}

/*
 * @brief 일시 정지된 소스의 재생을 재개함.
 * @param sourceId 재개할 OpenAL 소스 ID.
 */
void SoundManager::resumeSound(ALuint sourceId) {
    alSourcePlay(sourceId);
    AL_CHECK_ERROR();
}

/*
 * @brief 지정된 소스의 재생 상태(재생/일시정지)를 토글함.
 * @param sourceId 토글할 OpenAL 소스 ID.
 */
void SoundManager::togglePauseSound(ALuint sourceId) {
    ALint state;
    alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING) {
        alSourcePause(sourceId);
    } else if (state == AL_PAUSED) {
        alSourcePlay(sourceId);
    }
    AL_CHECK_ERROR();
}

/*
 * @brief 지정된 소스의 재생을 완전히 중지하고 보이스를 반환함.
 * @param sourceId 중지할 OpenAL 소스 ID.
 */
void SoundManager::stopSound(ALuint sourceId) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        releaseVoice(&(*it));
    }
}

/*
 * @brief 현재 재생 중인 모든 사운드를 중지함.
 */
void SoundManager::stopAllSounds() {
    for (auto& voice : voicePool_) {
        if (voice.isPlaying) {
            releaseVoice(&voice);
        }
    }
}

/*
 * @brief 재생할 비어있는 보이스(채널)를 찾거나, 필요 시 우선순위가 낮은 보이스를 훔침.
 * @param priority 새로 재생할 사운드의 우선순위.
 * @param forSplitStereo 3D 분리 스테레오 재생을 위한 보이스가 필요한지 여부.
 * @return 사용 가능한 Voice 객체의 포인터. 사용 가능한 보이스가 없으면 nullptr 반환.
 */
SoundManager::Voice* SoundManager::findAvailableVoice(SoundPriority priority, bool forSplitStereo) {
    // 1. 재생이 끝난 (비어있는) 보이스를 먼저 찾음.
    for (auto& voice : voicePool_) {
        ALint stateLeft;
        alGetSourcei(voice.sourceIdLeft, AL_SOURCE_STATE, &stateLeft);

        bool isAvailable = (stateLeft != AL_PLAYING && stateLeft != AL_PAUSED);

        // 3D 스테레오의 경우 오른쪽 채널도 확인
        if (voice.isSplitStereo) {
            ALint stateRight;
            alGetSourcei(voice.sourceIdRight, AL_SOURCE_STATE, &stateRight);
            if (stateRight == AL_PLAYING || stateRight == AL_PAUSED) {
                isAvailable = false;
            }
        }

        if (isAvailable) {
            voice.isPlaying = false; // isPlaying 플래그를 최신 상태로 업데이트.
            return &voice;
        }
    }

    // 2. 비어있는 보이스가 없으면, 우선순위가 낮은 보이스를 훔칠(steal) 것을 시도함.
    Voice* lowestPriorityVoice = nullptr;
    for (auto& voice : voicePool_) {
        // 새 사운드의 우선순위보다 낮거나 같은 보이스 중에서
        if (voice.priority <= priority) {
            // 가장 우선순위가 낮은 보이스를 찾음.
            if (!lowestPriorityVoice || voice.priority < lowestPriorityVoice->priority) {
                lowestPriorityVoice = &voice;
            }
        }
    }

    if (lowestPriorityVoice) {
        std::cout << "Stealing voice (source " << lowestPriorityVoice->sourceIdLeft << ") for new sound." << std::endl;
        releaseVoice(lowestPriorityVoice); // 찾은 보이스를 정리하고 반환.
        return lowestPriorityVoice;
    }

    // 3. 훔칠 보이스도 없음 (모든 보이스가 더 높은 우선순위의 사운드를 재생 중).
    std::cerr << "No available voice for priority " << static_cast<int>(priority) << std::endl;
    return nullptr;
}

/*
 * @brief 소스 ID를 사용하여 보이스를 해제(반환)함.
 * @param sourceId 해제할 보이스의 소스 ID.
 */
void SoundManager::releaseVoice(ALuint sourceId) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        releaseVoice(&(*it));
    }
}

/*
 * @brief Voice 객체를 직접 받아 보이스를 해제(반환)함.
 * @param voice 해제할 Voice 객체의 포인터.
 * @note 소스 재생을 멈추고, 연결된 버퍼를 해제하며, 상태를 초기화함.
 */
void SoundManager::releaseVoice(Voice* voice) {
    alSourceStop(voice->sourceIdLeft);
    alSourcei(voice->sourceIdLeft, AL_BUFFER, 0); // 버퍼 연결 해제.
    AL_CHECK_ERROR();

    if (voice->isSplitStereo) {
        alSourceStop(voice->sourceIdRight);
        alSourcei(voice->sourceIdRight, AL_BUFFER, 0);
        AL_CHECK_ERROR();
    }

    voice->isPlaying = false;
    voice->isSplitStereo = false;
    voice->priority = SoundPriority::LOW; // 우선순위를 기본값으로 초기화.
}


/*
 * @brief 리스너(청자)의 3D 공간상 위치를 설정함.
 * @param x, y, z 리스너의 월드 좌표.
 */
void SoundManager::setListenerPosition(float x, float y, float z) {
    alListener3f(AL_POSITION, x, y, z);
    AL_CHECK_ERROR();
}

/*
 * @brief 리스너(청자)의 방향을 설정함.
 * @param atX, atY, atZ 리스너가 바라보는 방향 벡터.
 * @param upX, upY, upZ 리스너의 머리 위쪽을 가리키는 상향 벡터.
 */
void SoundManager::setListenerOrientation(float atX, float atY, float atZ, float upX, float upY, float upZ) {
    ALfloat orientation[] = {atX, atY, atZ, upX, upY, upZ};
    alListenerfv(AL_ORIENTATION, orientation);
    AL_CHECK_ERROR();
}

/*
 * @brief 특정 소스의 3D 공간상 위치를 설정함.
 * @param sourceId 위치를 설정할 소스의 ID.
 * @param x, y, z 소스의 월드 좌표.
 * @note 3D 분리 스테레오 재생 시, 좌우 채널에 약간의 간격을 두어 공간감을 향상시킴.
 */
void SoundManager::setSourcePosition(ALuint sourceId, float x, float y, float z) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        // 3D 분리 스테레오 사운드인 경우, 좌우 채널에 약간의 오프셋을 적용.
        if (it->isSplitStereo) {
            const float stereo_separation = 0.5f; // 좌우 소스 간의 거리 (OpenAL 단위).
            alSource3f(it->sourceIdLeft, AL_POSITION, x - stereo_separation, y, z);
            alSource3f(it->sourceIdRight, AL_POSITION, x + stereo_separation, y, z);
        } else {
            alSource3f(it->sourceIdLeft, AL_POSITION, x, y, z);
        }
        AL_CHECK_ERROR();
    }
}

/*
 * @brief 특정 소스의 볼륨을 설정함.
 * @param sourceId 볼륨을 설정할 소스의 ID.
 * @param volume 새로운 볼륨 값 (0.0 ~ 1.0).
 */
void SoundManager::setSourceVolume(ALuint sourceId, float volume) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        alSourcef(it->sourceIdLeft, AL_GAIN, volume);
        if (it->isSplitStereo) {
            alSourcef(it->sourceIdRight, AL_GAIN, volume);
        }
        AL_CHECK_ERROR();
    }
}

/*
 * @brief 특정 소스의 피치(재생 속도)를 설정함.
 * @param sourceId 피치를 설정할 소스의 ID.
 * @param pitch 새로운 피치 값.
 */
void SoundManager::setSourcePitch(ALuint sourceId, float pitch) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        alSourcef(it->sourceIdLeft, AL_PITCH, pitch);
        if (it->isSplitStereo) {
            alSourcef(it->sourceIdRight, AL_PITCH, pitch);
        }
        AL_CHECK_ERROR();
    }
}

/*
 * @brief 특정 소스의 거리 감쇠율(Rolloff Factor)을 설정함.
 * @param sourceId 설정할 소스의 ID.
 * @param factor 새로운 감쇠율. 값이 클수록 거리에 따라 볼륨이 더 빨리 줄어듦.
 */
void SoundManager::setSourceRolloffFactor(ALuint sourceId, float factor) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        alSourcef(it->sourceIdLeft, AL_ROLLOFF_FACTOR, factor);
        if (it->isSplitStereo) {
            alSourcef(it->sourceIdRight, AL_ROLLOFF_FACTOR, factor);
        }
        AL_CHECK_ERROR();
    }
}

/*
 * @brief 특정 소스의 기준 거리(Reference Distance)를 설정함.
 * @param sourceId 설정할 소스의 ID.
 * @param distance 새로운 기준 거리. 이 거리에서 소리는 최대 볼륨으로 들림.
 */
void SoundManager::setSourceReferenceDistance(ALuint sourceId, float distance) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        alSourcef(it->sourceIdLeft, AL_REFERENCE_DISTANCE, distance);
        if (it->isSplitStereo) {
            alSourcef(it->sourceIdRight, AL_REFERENCE_DISTANCE, distance);
        }
        AL_CHECK_ERROR();
    }
}

/*
 * @brief 특정 소스의 최대 거리(Max Distance)를 설정함.
 * @param sourceId 설정할 소스의 ID.
 * @param distance 새로운 최대 거리. 이 거리를 넘어서면 더 이상 소리가 감쇠하지 않음.
 */
void SoundManager::setSourceMaxDistance(ALuint sourceId, float distance) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        alSourcef(it->sourceIdLeft, AL_MAX_DISTANCE, distance);
        if (it->isSplitStereo) {
            alSourcef(it->sourceIdRight, AL_MAX_DISTANCE, distance);
        }
        AL_CHECK_ERROR();
    }
}

/*
 * @brief 3D 분리 스테레오 재생 시, 왼쪽 채널 소스 ID를 이용해 오른쪽 채널 소스 ID를 가져옴.
 * @param sourceIdLeft 왼쪽 채널의 소스 ID.
 * @return 오른쪽 채널의 소스 ID. 해당 보이스가 3D 분리 스테레오가 아니면 0을 반환.
 */
ALuint SoundManager::getSourceIdRight(ALuint sourceIdLeft) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceIdLeft](const Voice& v) {
        return v.sourceIdLeft == sourceIdLeft;
    });

    if (it != voicePool_.end() && it->isSplitStereo) {
        return it->sourceIdRight;
    }
    return 0;
}

/*
 * @brief WAV 파일을 디코딩하고 OpenAL 버퍼에 데이터를 채움.
 * @param filePath WAV 파일 경로.
 * @param bufferInfo 버퍼 정보를 저장할 구조체.
 * @return 로딩 성공 시 true, 실패 시 false.
 * @note 스테레오 파일의 경우, 3D용 분리 모노 버퍼 2개와 2D용 통합 스테레오 버퍼 1개를 모두 생성함.
 */
bool SoundManager::loadWav(const std::filesystem::path& filePath, SoundBufferInfo& bufferInfo) {
    unsigned int channels;
    unsigned int sampleRate;
    drwav_uint64 totalPcmFrameCount;
    short* pData = drwav_open_file_and_read_pcm_frames_s16(filePath.string().c_str(), &channels, &sampleRate, &totalPcmFrameCount, NULL);
    if (pData == NULL) {
        std::cerr << "Failed to load WAV file: " << filePath.string() << std::endl;
        return false;
    }

    if (channels == 1) { // 모노
        alGenBuffers(1, &bufferInfo.monoBufferLeft);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBufferLeft, AL_FORMAT_MONO16, pData, totalPcmFrameCount * channels * sizeof(short), sampleRate);
        bufferInfo.isStereo = false;
    } else if (channels == 2) { // 스테레오
        bufferInfo.isStereo = true;

        // 1. 2D 재생을 위한 통합 스테레오 버퍼 생성
        alGenBuffers(1, &bufferInfo.stereoBuffer);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.stereoBuffer, AL_FORMAT_STEREO16, pData, totalPcmFrameCount * channels * sizeof(short), sampleRate);
        AL_CHECK_ERROR();

        // 2. 3D 재생을 위한 채널 분리 및 모노 버퍼 생성
        std::vector<short> pcmLeft(totalPcmFrameCount);
        std::vector<short> pcmRight(totalPcmFrameCount);
        for (drwav_uint64 i = 0; i < totalPcmFrameCount; ++i) {
            pcmLeft[i] = pData[i * 2];
            pcmRight[i] = pData[i * 2 + 1];
        }

        alGenBuffers(1, &bufferInfo.monoBufferLeft);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBufferLeft, AL_FORMAT_MONO16, pcmLeft.data(), pcmLeft.size() * sizeof(short), sampleRate);
        AL_CHECK_ERROR();

        alGenBuffers(1, &bufferInfo.monoBufferRight);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBufferRight, AL_FORMAT_MONO16, pcmRight.data(), pcmRight.size() * sizeof(short), sampleRate);
        AL_CHECK_ERROR();
    } else {
        std::cerr << "Unsupported WAV channel count: " << channels << "\n";
        drwav_free(pData, NULL);
        return false;
    }

    drwav_free(pData, NULL);
    return true;
}

/*
 * @brief MP3 파일을 디코딩하고 OpenAL 버퍼에 데이터를 채움.
 * @param filePath MP3 파일 경로.
 * @param bufferInfo 버퍼 정보를 저장할 구조체.
 * @return 로딩 성공 시 true, 실패 시 false.
 */
bool SoundManager::loadMp3(const std::filesystem::path& filePath, SoundBufferInfo& bufferInfo) {
    drmp3_config config;
    drmp3_uint64 totalPcmFrameCount;
    float* pPcmData = drmp3_open_file_and_read_pcm_frames_f32(filePath.string().c_str(), &config, &totalPcmFrameCount, NULL);
    if (pPcmData == NULL) {
        std::cerr << "Failed to load MP3 file: " << filePath.string() << "\n";
        return false;
    }

    std::vector<short> pcm16;
    pcm16.reserve(totalPcmFrameCount * config.channels);
    for (drmp3_uint64 i = 0; i < totalPcmFrameCount * config.channels; ++i) {
        pcm16.push_back(static_cast<short>(pPcmData[i] * 32767.0f));
    }
    drmp3_free(pPcmData, NULL);

    if (config.channels == 1) { // 모노
        bufferInfo.isStereo = false;
        alGenBuffers(1, &bufferInfo.monoBufferLeft);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBufferLeft, AL_FORMAT_MONO16, pcm16.data(), pcm16.size() * sizeof(short), config.sampleRate);
        AL_CHECK_ERROR();
    } else if (config.channels == 2) { // 스테레오
        bufferInfo.isStereo = true;

        // 1. 2D 재생을 위한 통합 스테레오 버퍼 생성
        alGenBuffers(1, &bufferInfo.stereoBuffer);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.stereoBuffer, AL_FORMAT_STEREO16, pcm16.data(), pcm16.size() * sizeof(short), config.sampleRate);
        AL_CHECK_ERROR();

        // 2. 3D 재생을 위한 채널 분리 및 모노 버퍼 생성
        std::vector<short> pcmLeft(totalPcmFrameCount);
        std::vector<short> pcmRight(totalPcmFrameCount);
        for (drmp3_uint64 i = 0; i < totalPcmFrameCount; ++i) {
            pcmLeft[i] = pcm16[i * 2];
            pcmRight[i] = pcm16[i * 2 + 1];
        }

        alGenBuffers(1, &bufferInfo.monoBufferLeft);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBufferLeft, AL_FORMAT_MONO16, pcmLeft.data(), pcmLeft.size() * sizeof(short), config.sampleRate);
        AL_CHECK_ERROR();

        alGenBuffers(1, &bufferInfo.monoBufferRight);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBufferRight, AL_FORMAT_MONO16, pcmRight.data(), pcmRight.size() * sizeof(short), config.sampleRate);
        AL_CHECK_ERROR();
    } else {
        std::cerr << "Unsupported MP3 channel count: " << config.channels << "\n";
        return false;
    }
    return true;
}

/*
 * @brief FLAC 파일을 디코딩하고 OpenAL 버퍼에 데이터를 채움.
 * @param filePath FLAC 파일 경로.
 * @param bufferInfo 버퍼 정보를 저장할 구조체.
 * @return 로딩 성공 시 true, 실패 시 false.
 */
bool SoundManager::loadFlac(const std::filesystem::path& filePath, SoundBufferInfo& bufferInfo) {
    unsigned int channels;
    unsigned int sampleRate;
    drflac_uint64 totalPcmFrameCount;
    short* pData = drflac_open_file_and_read_pcm_frames_s16(filePath.string().c_str(), &channels, &sampleRate, &totalPcmFrameCount, NULL);
    if (pData == NULL) {
        std::cerr << "Failed to load FLAC file: " << filePath.string() << std::endl;
        return false;
    }

    if (channels == 1) { // 모노
        bufferInfo.isStereo = false;
        alGenBuffers(1, &bufferInfo.monoBufferLeft);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBufferLeft, AL_FORMAT_MONO16, pData, totalPcmFrameCount * channels * sizeof(short), sampleRate);
        AL_CHECK_ERROR();
    } else if (channels == 2) { // 스테레오
        bufferInfo.isStereo = true;

        // 1. 2D 재생을 위한 통합 스테레오 버퍼 생성
        alGenBuffers(1, &bufferInfo.stereoBuffer);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.stereoBuffer, AL_FORMAT_STEREO16, pData, totalPcmFrameCount * channels * sizeof(short), sampleRate);
        AL_CHECK_ERROR();

        // 2. 3D 재생을 위한 채널 분리 및 모노 버퍼 생성
        std::vector<short> pcmLeft(totalPcmFrameCount);
        std::vector<short> pcmRight(totalPcmFrameCount);
        for (drflac_uint64 i = 0; i < totalPcmFrameCount; ++i) {
            pcmLeft[i] = pData[i * 2];
            pcmRight[i] = pData[i * 2 + 1];
        }

        alGenBuffers(1, &bufferInfo.monoBufferLeft);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBufferLeft, AL_FORMAT_MONO16, pcmLeft.data(), pcmLeft.size() * sizeof(short), sampleRate);
        AL_CHECK_ERROR();

        alGenBuffers(1, &bufferInfo.monoBufferRight);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBufferRight, AL_FORMAT_MONO16, pcmRight.data(), pcmRight.size() * sizeof(short), sampleRate);
        AL_CHECK_ERROR();
    } else {
        std::cerr << "Unsupported FLAC channel count: " << channels << "\n";
        drflac_free(pData, NULL);
        return false;
    }

    drflac_free(pData, NULL);
    return true;
}

/*
 * @brief OGG 파일을 디코딩하고 OpenAL 버퍼에 데이터를 채움.
 * @param filePath OGG 파일 경로.
 * @param bufferInfo 버퍼 정보를 저장할 구조체.
 * @return 로딩 성공 시 true, 실패 시 false.
 */
bool SoundManager::loadOgg(const std::filesystem::path& filePath, SoundBufferInfo& bufferInfo) {
    int channels;
    int sample_rate;
    short* pcm;
    int samples = stb_vorbis_decode_filename(filePath.string().c_str(), &channels, &sample_rate, &pcm);

    if (samples == -1) {
        std::cerr << "Failed to load OGG file: " << filePath.string() << "\n";
        return false;
    }

    if (channels == 1) { // 모노
        bufferInfo.isStereo = false;
        alGenBuffers(1, &bufferInfo.monoBufferLeft);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBufferLeft, AL_FORMAT_MONO16, pcm, samples * channels * sizeof(short), sample_rate);
        AL_CHECK_ERROR();
    } else if (channels == 2) { // 스테레오
        bufferInfo.isStereo = true;

        // 1. 2D 재생을 위한 통합 스테레오 버퍼 생성
        alGenBuffers(1, &bufferInfo.stereoBuffer);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.stereoBuffer, AL_FORMAT_STEREO16, pcm, samples * channels * sizeof(short), sample_rate);
        AL_CHECK_ERROR();

        // 2. 3D 재생을 위한 채널 분리 및 모노 버퍼 생성
        std::vector<short> pcmLeft(samples);
        std::vector<short> pcmRight(samples);
        for (int i = 0; i < samples; ++i) {
            pcmLeft[i] = pcm[i * 2];
            pcmRight[i] = pcm[i * 2 + 1];
        }

        alGenBuffers(1, &bufferInfo.monoBufferLeft);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBufferLeft, AL_FORMAT_MONO16, pcmLeft.data(), pcmLeft.size() * sizeof(short), sample_rate);
        AL_CHECK_ERROR();

        alGenBuffers(1, &bufferInfo.monoBufferRight);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBufferRight, AL_FORMAT_MONO16, pcmRight.data(), pcmRight.size() * sizeof(short), sample_rate);
        AL_CHECK_ERROR();
    } else {
        std::cerr << "Unsupported OGG channel count: " << channels << "\n";
        free(pcm);
        return false;
    }

    free(pcm);
    return true;
}
