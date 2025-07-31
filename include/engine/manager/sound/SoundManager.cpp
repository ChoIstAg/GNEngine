#include "SoundManager.h"
#include <iostream>
#include <algorithm> // for std::find_if
#include <filesystem>

#include <AL/alc.h>
#include <AL/alext.h> // OpenAL 확장 헤더 포함
// dr_libs
#define DR_MP3_IMPLEMENTATION
#include "dr_libs/dr_mp3.h"
#define DR_WAV_IMPLEMENTATION
#include "dr_libs/dr_wav.h"
#define DR_FLAC_IMPLEMENTATION
#include "dr_libs/dr_flac.h"

#include "ogg/stb_vorbis.c"

SoundManager::SoundManager() {
    initAL();
}

SoundManager::~SoundManager() {
    quitAL();
}

bool SoundManager::initAL() {
    device_ = alcOpenDevice(NULL);
    if (!device_) {
        std::cerr << "Failed to open OpenAL device(" << alcGetString(NULL, alcGetError(NULL)) << ")." << std::endl;
        return false;
    }
    
    // HRTF를 활성화하기 위한 속성
    ALCint attr[] = {
        ALC_HRTF_SOFT, ALC_TRUE,
        0
    };

    // 사용 가능한 HRTF를 쿼리하고 설정.
    const ALCchar* hrtf_specifier = alcGetString(device_, ALC_HRTF_SPECIFIER_SOFT);
    if (hrtf_specifier && *hrtf_specifier) {
        std::cout << "Available HRTF specifiers: " << hrtf_specifier << std::endl;
        // 첫 번째 HRTF를 선택하여 사용
        attr[2] = ALC_HRTF_ID_SOFT;
        attr[3] = 1; // 첫 번째 HRTF ID (OpenAL Soft는 1부터 시작)
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
    
    if (!alcMakeContextCurrent(context_)) {
        std::cerr << "Failed to make OpenAL context current." << std::endl;
        alcDestroyContext(context_);
        alcCloseDevice(device_);
        context_ = nullptr;
        device_ = nullptr;
        return false;
    }
    
    AL_CHECK_ERROR();

    // OpenAL 거리 모델 설정
    // AL_INVERSE_DISTANCE_CLAMPED: 거리에 따라 볼륨이 감소하지만, 특정 거리 이하로 내려가지 않음
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    AL_CHECK_ERROR();

    // 리스너 기본 설정 (위치: 0,0,0), (정면, 상향 Y축)
    setListenerPosition(0.0f, 0.0f, 0.0f);
    setListenerOrientation(0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

    // 보이스 풀 초기화
    voicePool_.resize(MAX_VOICES);
    for (int i = 0; i < MAX_VOICES; ++i) {
        alGenSources(1, &voicePool_[i].sourceIdLeft);
        AL_CHECK_ERROR();
        if (voicePool_[i].sourceIdLeft == 0) {
            std::cerr << "Failed to generate OpenAL left source." << std::endl;
            // 이미 생성된 소스들 정리
            for (int j = 0; j < i; ++j) {
                alDeleteSources(1, &voicePool_[j].sourceIdLeft);
                alDeleteSources(1, &voicePool_[j].sourceIdRight);
            }
            return false;
        }
        alGenSources(1, &voicePool_[i].sourceIdRight);
        AL_CHECK_ERROR();
        if (voicePool_[i].sourceIdRight == 0) {
            std::cerr << "Failed to generate OpenAL right source." << std::endl;
            alDeleteSources(1, &voicePool_[i].sourceIdLeft);
            // 이미 생성된 소스들 정리
            for (int j = 0; j < i; ++j) {
                alDeleteSources(1, &voicePool_[j].sourceIdLeft);
                alDeleteSources(1, &voicePool_[j].sourceIdRight);
            }
            return false;
        }

        // 각 소스의 기본 감쇠 설정 (선택 사항, 개별 소스에 적용 가능)
        alSourcef(voicePool_[i].sourceIdLeft, AL_REFERENCE_DISTANCE, 1.0f); // 1.0 단위 거리에서 최대 볼륨
        alSourcef(voicePool_[i].sourceIdLeft, AL_MAX_DISTANCE, 100.0f); // 100.0 단위 거리에서 더 이상 감쇠하지 않음
        alSourcef(voicePool_[i].sourceIdLeft, AL_ROLLOFF_FACTOR, 1.0f); // 감쇠율 (기본값 1.0)
        alSourcef(voicePool_[i].sourceIdRight, AL_REFERENCE_DISTANCE, 1.0f); // 1.0 단위 거리에서 최대 볼륨
        alSourcef(voicePool_[i].sourceIdRight, AL_MAX_DISTANCE, 100.0f); // 100.0 단위 거리에서 더 이상 감쇠하지 않음
        alSourcef(voicePool_[i].sourceIdRight, AL_ROLLOFF_FACTOR, 1.0f); // 감쇠율 (기본값 1.0)
        AL_CHECK_ERROR();
    }

    std::cout << "OpenAL initialized with " << MAX_VOICES << " voices." << std::endl;
    return true;
}

void SoundManager::quitAL() {
    if (context_ && device_) {
        alcMakeContextCurrent(context_);
        AL_CHECK_ERROR();

        stopAllSounds();

        // 보이스 풀의 모든 소스 삭제
        for (auto& voice : voicePool_) {
            if (alIsSource(voice.sourceIdLeft)) {
                alDeleteSources(1, &voice.sourceIdLeft);
            }
            if (alIsSource(voice.sourceIdRight)) {
                alDeleteSources(1, &voice.sourceIdRight);
            }
        }
        voicePool_.clear();

        // 모든 버퍼 삭제
        for (auto const& [id, bufferInfo] : soundBuffers_) {
            if (bufferInfo.monoBuffer != 0 && alIsBuffer(bufferInfo.monoBuffer)) {
                alDeleteBuffers(1, &bufferInfo.monoBuffer);
            }
            if (bufferInfo.isStereo && bufferInfo.stereoBufferRight != 0 && alIsBuffer(bufferInfo.stereoBufferRight)) {
                alDeleteBuffers(1, &bufferInfo.stereoBufferRight);
            }
        }
        soundBuffers_.clear();

        alcMakeContextCurrent(NULL);
        alcDestroyContext(context_);
        context_ = nullptr;

        alcCloseDevice(device_);
        device_ = nullptr;
    }
    std::cout << "OpenAL shut down." << std::endl;
}

void SoundManager::checkAlErrors(const std::string& filename, int line) {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "OpenAL Error in " << filename << " (" << line << "): " << alGetString(error) << std::endl;
    }
}

bool SoundManager::loadSound(const std::string& filePath) {
    if (soundBuffers_.count(filePath)) {
        return true; // 이미 로드됨
    }

    SoundBufferInfo bufferInfo;
    
    std::filesystem::path path(filePath);
    std::string extension = path.extension().string();
    
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
        // 로딩 실패 시 생성된 버퍼 삭제
        if (bufferInfo.monoBuffer != 0) alDeleteBuffers(1, &bufferInfo.monoBuffer);
        if (bufferInfo.stereoBufferRight != 0) alDeleteBuffers(1, &bufferInfo.stereoBufferRight);
        return false;
    }
}

ALuint SoundManager::playSound(const std::string& filePath, SoundPriority priority, float volume, float pitch, bool loop) {
    auto it = soundBuffers_.find(filePath);
    if (it == soundBuffers_.end()) {
        std::cerr << "Sound with ID '" << filePath << "' not found." << std::endl;
        return 0;
    }

    Voice* voice = findAvailableVoice(priority, it->second.isStereo);
    if (!voice) {
        // 재생할 보이스가 없음. (요청한 우선순위가 사용중인 소스에 비해 낮음.)
        return 0;
    }

    voice->soundPath = filePath;
    voice->priority = priority;
    voice->isPlaying = true;
    voice->isStereo = it->second.isStereo;

    ALuint sourceLeft = voice->sourceIdLeft;
    ALuint sourceRight = voice->sourceIdRight;

    if (voice->isStereo) {
        alSourcei(sourceLeft, AL_BUFFER, it->second.monoBuffer); // Left channel
        alSourcef(sourceLeft, AL_GAIN, volume); // Apply volume to left channel
        alSourcef(sourceLeft, AL_PITCH, pitch); // Apply pitch to left channel
        alSourcei(sourceLeft, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        AL_CHECK_ERROR();

        alSourcei(sourceRight, AL_BUFFER, it->second.stereoBufferRight); // Right channel
        alSourcef(sourceRight, AL_GAIN, volume); // Apply volume to right channel
        alSourcef(sourceRight, AL_PITCH, pitch); // Apply pitch to right channel
        alSourcei(sourceRight, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        AL_CHECK_ERROR();

        alSourcePlay(sourceLeft);
        alSourcePlay(sourceRight);
        AL_CHECK_ERROR();
    } else {
        alSourcei(sourceLeft, AL_BUFFER, it->second.monoBuffer);
        alSourcef(sourceLeft, AL_GAIN, volume);
        alSourcef(sourceLeft, AL_PITCH, pitch);
        alSourcei(sourceLeft, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        AL_CHECK_ERROR();

        alSourcePlay(sourceLeft);
        AL_CHECK_ERROR();
    }

    return sourceLeft;
}

/* @brief 소리를 완전 정지함.
* @param sourceId OpenAL 소스 ID.
* @note 이 함수는 소스를 풀로 반환하는 대신 상태를 '사용 가능'으로 변경합니다.
*/
void SoundManager::pauseSound(ALuint sourceId) {
    alSourcePause(sourceId);
    AL_CHECK_ERROR();
}

void SoundManager::resumeSound(ALuint sourceId) {
    alSourcePlay(sourceId);
    AL_CHECK_ERROR();
}

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

void SoundManager::stopSound(ALuint sourceId) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        releaseVoice(&(*it));
    }
}

void SoundManager::stopAllSounds() {
    for (auto& voice : voicePool_) {
        if (voice.isPlaying) {
            releaseVoice(&voice);
        }
    }
}

SoundManager::Voice* SoundManager::findAvailableVoice(SoundPriority priority, bool forStereo) {
    // 1. 사용 가능한 (재생 중이 아닌) 보이스를 찾음
    for (auto& voice : voicePool_) {
        ALint stateLeft, stateRight;
        alGetSourcei(voice.sourceIdLeft, AL_SOURCE_STATE, &stateLeft);
        alGetSourcei(voice.sourceIdRight, AL_SOURCE_STATE, &stateRight);

        if (stateLeft != AL_PLAYING && stateLeft != AL_PAUSED &&
            stateRight != AL_PLAYING && stateRight != AL_PAUSED) {
            voice.isPlaying = false; // 상태 갱신
        }

        if (!voice.isPlaying) {
            return &voice;
        }
    }

    // 2. 모든 보이스가 사용 중이면, 보이스 훔치기 시도
    Voice* lowestPriorityVoice = nullptr;
    for (auto& voice : voicePool_) {
        if (voice.priority <= priority) {
            if (!lowestPriorityVoice || voice.priority < lowestPriorityVoice->priority) {
                lowestPriorityVoice = &voice;
            }
        }
    }

    if (lowestPriorityVoice) {
        std::cout << "Stealing voice (source " << lowestPriorityVoice->sourceIdLeft << ") for new sound." << std::endl;
        releaseVoice(lowestPriorityVoice);
        return lowestPriorityVoice;
    }

    // 3. 훔칠 보이스도 없음 (새 사운드의 우선순위가 너무 낮음)
    std::cerr << "No available voice for priority " << static_cast<int>(priority) << std::endl;
    return nullptr;
}

void SoundManager::releaseVoice(ALuint sourceId) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        releaseVoice(&(*it));
    }
}

void SoundManager::releaseVoice(Voice* voice) {
    alSourceStop(voice->sourceIdLeft);
    alSourcei(voice->sourceIdLeft, AL_BUFFER, 0); // 버퍼 연결 해제
    AL_CHECK_ERROR();

    if (voice->isStereo) {
        alSourceStop(voice->sourceIdRight);
        alSourcei(voice->sourceIdRight, AL_BUFFER, 0); // 버퍼 연결 해제
        AL_CHECK_ERROR();
    }

    voice->isPlaying = false;
    voice->isStereo = false;
    voice->priority = SoundPriority::LOW; // 우선순위 초기화
}


void SoundManager::setListenerPosition(float x, float y, float z) {
    alListener3f(AL_POSITION, x, y, z);
    AL_CHECK_ERROR();
}

void SoundManager::setListenerOrientation(float atX, float atY, float atZ, float upX, float upY, float upZ) {
    ALfloat orientation[] = {atX, atY, atZ, upX, upY, upZ};
    alListenerfv(AL_ORIENTATION, orientation);
    AL_CHECK_ERROR();
}

void SoundManager::setSourcePosition(ALuint sourceId, float x, float y, float z) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        // 스테레오 사운드인 경우, 좌우 채널에 약간의 오프셋을 적용
        if (it->isStereo) {
            // 이 오프셋 값은 공간감 체감에 중요합니다. 조절 가능.
            const float stereo_separation = 0.5f; // 좌우 소스 간의 거리 (OpenAL 단위)
            alSource3f(it->sourceIdLeft, AL_POSITION, x - stereo_separation, y, z);
            alSource3f(it->sourceIdRight, AL_POSITION, x + stereo_separation, y, z);
        } else {
            alSource3f(sourceId, AL_POSITION, x, y, z);
        }
        AL_CHECK_ERROR();
    }
}

void SoundManager::setSourceVolume(ALuint sourceId, float volume) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        alSourcef(it->sourceIdLeft, AL_GAIN, volume);
        if (it->isStereo) {
            alSourcef(it->sourceIdRight, AL_GAIN, volume);
        }
        AL_CHECK_ERROR();
    }
}

void SoundManager::setSourcePitch(ALuint sourceId, float pitch) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        alSourcef(it->sourceIdLeft, AL_PITCH, pitch);
        if (it->isStereo) {
            alSourcef(it->sourceIdRight, AL_PITCH, pitch);
        }
        AL_CHECK_ERROR();
    }
}

void SoundManager::setSourceRolloffFactor(ALuint sourceId, float factor) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        alSourcef(it->sourceIdLeft, AL_ROLLOFF_FACTOR, factor);
        if (it->isStereo) {
            alSourcef(it->sourceIdRight, AL_ROLLOFF_FACTOR, factor);
        }
        AL_CHECK_ERROR();
    }
}

void SoundManager::setSourceReferenceDistance(ALuint sourceId, float distance) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        alSourcef(it->sourceIdLeft, AL_REFERENCE_DISTANCE, distance);
        if (it->isStereo) {
            alSourcef(it->sourceIdRight, AL_REFERENCE_DISTANCE, distance);
        }
        AL_CHECK_ERROR();
    }
}

void SoundManager::setSourceMaxDistance(ALuint sourceId, float distance) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceId](const Voice& v) {
        return v.sourceIdLeft == sourceId || v.sourceIdRight == sourceId;
    });

    if (it != voicePool_.end()) {
        alSourcef(it->sourceIdLeft, AL_MAX_DISTANCE, distance);
        if (it->isStereo) {
            alSourcef(it->sourceIdRight, AL_MAX_DISTANCE, distance);
        }
        AL_CHECK_ERROR();
    }
}

ALuint SoundManager::getSourceIdRight(ALuint sourceIdLeft) {
    auto it = std::find_if(voicePool_.begin(), voicePool_.end(), [sourceIdLeft](const Voice& v) {
        return v.sourceIdLeft == sourceIdLeft;
    });

    if (it != voicePool_.end() && it->isStereo) {
        return it->sourceIdRight;
    }
    return 0;
}

// WAV 파일 로딩
bool SoundManager::loadWav(const std::string& filePath, SoundBufferInfo& bufferInfo) {
    unsigned int channels;
    unsigned int sampleRate;
    drwav_uint64 totalPcmFrameCount;
    short* pData = drwav_open_file_and_read_pcm_frames_s16(filePath.c_str(), &channels, &sampleRate, &totalPcmFrameCount, NULL);
    if (pData == NULL) {
        std::cerr << "Failed to load WAV file: " << filePath << std::endl;
        return false;
    }

    alGenBuffers(1, &bufferInfo.monoBuffer);
    AL_CHECK_ERROR();

    if (channels == 1) {
        alBufferData(bufferInfo.monoBuffer, AL_FORMAT_MONO16, pData, totalPcmFrameCount * channels * sizeof(short), sampleRate);
        bufferInfo.isStereo = false;
    } else if (channels == 2) {
        // 스테레오 사운드: 왼쪽/오른쪽 채널 분리
        std::vector<short> pcmLeft(totalPcmFrameCount);
        std::vector<short> pcmRight(totalPcmFrameCount);
        for (drwav_uint64 i = 0; i < totalPcmFrameCount; ++i) {
            pcmLeft[i] = pData[i * 2];
            pcmRight[i] = pData[i * 2 + 1];
        }

        alBufferData(bufferInfo.monoBuffer, AL_FORMAT_MONO16, pcmLeft.data(), pcmLeft.size() * sizeof(short), sampleRate);
        AL_CHECK_ERROR();

        alGenBuffers(1, &bufferInfo.stereoBufferRight);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.stereoBufferRight, AL_FORMAT_MONO16, pcmRight.data(), pcmRight.size() * sizeof(short), sampleRate);
        AL_CHECK_ERROR();
        bufferInfo.isStereo = true;
    } else {
        std::cerr << "Unsupported WAV channel count: " << channels << "\n";
        drwav_free(pData, NULL);
        return false;
    }

    drwav_free(pData, NULL);
    return true;
}

// MP3 파일 로딩
bool SoundManager::loadMp3(const std::string& filePath, SoundBufferInfo& bufferInfo) {
    drmp3_config config;
    drmp3_uint64 totalPcmFrameCount;
    float* pPcmData = drmp3_open_file_and_read_pcm_frames_f32(filePath.c_str(), &config, &totalPcmFrameCount, NULL);
    if (pPcmData == NULL) {
        std::cerr << "Failed to load MP3 file: " << filePath << "\n";
        return false;
    }

    std::vector<short> pcm16;
    pcm16.reserve(totalPcmFrameCount * config.channels);
    for (drmp3_uint64 i = 0; i < totalPcmFrameCount * config.channels; ++i) {
        pcm16.push_back(static_cast<short>(pPcmData[i] * 32767.0f));
    }
    drmp3_free(pPcmData, NULL);

    ALenum alFormat;
    if (config.channels == 1) {
        alFormat = AL_FORMAT_MONO16;
    } else if (config.channels == 2) {
        // 스테레오 사운드: 왼쪽/오른쪽 채널 분리
        std::vector<short> pcmLeft(totalPcmFrameCount);
        std::vector<short> pcmRight(totalPcmFrameCount);
        for (drmp3_uint64 i = 0; i < totalPcmFrameCount; ++i) {
            pcmLeft[i] = pcm16[i * 2];
            pcmRight[i] = pcm16[i * 2 + 1];
        }

        alGenBuffers(1, &bufferInfo.monoBuffer);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBuffer, AL_FORMAT_MONO16, pcmLeft.data(), pcmLeft.size() * sizeof(short), config.sampleRate);
        AL_CHECK_ERROR();

        alGenBuffers(1, &bufferInfo.stereoBufferRight);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.stereoBufferRight, AL_FORMAT_MONO16, pcmRight.data(), pcmRight.size() * sizeof(short), config.sampleRate);
        AL_CHECK_ERROR();
        bufferInfo.isStereo = true;
    } else {
        alFormat = AL_FORMAT_MONO16;
        alGenBuffers(1, &bufferInfo.monoBuffer);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.monoBuffer, alFormat, pcm16.data(), pcm16.size() * sizeof(short), config.sampleRate);
        AL_CHECK_ERROR();
        bufferInfo.isStereo = false;
    }
    return true;
}

bool SoundManager::loadFlac(const std::string& filePath, SoundBufferInfo& bufferInfo) {
    unsigned int channels;
    unsigned int sampleRate;
    drflac_uint64 totalPcmFrameCount;
    short* pData = drflac_open_file_and_read_pcm_frames_s16(filePath.c_str(), &channels, &sampleRate, &totalPcmFrameCount, NULL);
    if (pData == NULL) {
        std::cerr << "Failed to load FLAC file: " << filePath << std::endl;
        return false;
    }

    alGenBuffers(1, &bufferInfo.monoBuffer);
    AL_CHECK_ERROR();

    if (channels == 1) {
        alBufferData(bufferInfo.monoBuffer, AL_FORMAT_MONO16, pData, totalPcmFrameCount * channels * sizeof(short), sampleRate);
        bufferInfo.isStereo = false;
    } else if (channels == 2) {
        // 스테레오 사운드: 왼쪽/오른쪽 채널 분리
        std::vector<short> pcmLeft(totalPcmFrameCount);
        std::vector<short> pcmRight(totalPcmFrameCount);
        for (drflac_uint64 i = 0; i < totalPcmFrameCount; ++i) {
            pcmLeft[i] = pData[i * 2];
            pcmRight[i] = pData[i * 2 + 1];
        }

        alBufferData(bufferInfo.monoBuffer, AL_FORMAT_MONO16, pcmLeft.data(), pcmLeft.size() * sizeof(short), sampleRate);
        AL_CHECK_ERROR();

        alGenBuffers(1, &bufferInfo.stereoBufferRight);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.stereoBufferRight, AL_FORMAT_MONO16, pcmRight.data(), pcmRight.size() * sizeof(short), sampleRate);
        AL_CHECK_ERROR();
        bufferInfo.isStereo = true;
    } else {
        std::cerr << "Unsupported FLAC channel count: " << channels << "\n";
        drflac_free(pData, NULL);
        return false;
    }

    drflac_free(pData, NULL);
    return true;
}

// OGG 파일 로딩
bool SoundManager::loadOgg(const std::string& filePath, SoundBufferInfo& bufferInfo) {
    int channels;
    int sample_rate;
    short* pcm;
    int samples = stb_vorbis_decode_filename(filePath.c_str(), &channels, &sample_rate, &pcm);

    if (samples == -1) {
        std::cerr << "Failed to load OGG file: " << filePath << "\n";
        return false;
    }

    alGenBuffers(1, &bufferInfo.monoBuffer);
    AL_CHECK_ERROR();

    if (channels == 1) {
        alBufferData(bufferInfo.monoBuffer, AL_FORMAT_MONO16, pcm, samples * channels * sizeof(short), sample_rate);
        bufferInfo.isStereo = false;
    } else if (channels == 2) {
        // 스테레오 사운드: 왼쪽/오른쪽 채널 분리
        std::vector<short> pcmLeft(samples);
        std::vector<short> pcmRight(samples);
        for (int i = 0; i < samples; ++i) {
            pcmLeft[i] = pcm[i * 2];
            pcmRight[i] = pcm[i * 2 + 1];
        }

        alBufferData(bufferInfo.monoBuffer, AL_FORMAT_MONO16, pcmLeft.data(), pcmLeft.size() * sizeof(short), sample_rate);
        AL_CHECK_ERROR();

        alGenBuffers(1, &bufferInfo.stereoBufferRight);
        AL_CHECK_ERROR();
        alBufferData(bufferInfo.stereoBufferRight, AL_FORMAT_MONO16, pcmRight.data(), pcmRight.size() * sizeof(short), sample_rate);
        AL_CHECK_ERROR();
        bufferInfo.isStereo = true;
    } else {
        std::cerr << "Unsupported OGG channel count: " << channels << "\n";
        free(pcm);
        return false;
    }

    free(pcm);
    return true;
}