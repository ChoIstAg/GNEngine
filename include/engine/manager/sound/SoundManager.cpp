#include "SoundManager.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

// dr_libs
#define DR_MP3_IMPLEMENTATION
#include "dr_libs/dr_mp3.h"
#define DR_WAV_IMPLEMENTATION
#include "dr_libs/dr_wav.h"

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
    
    context_ = alcCreateContext(device_, NULL);
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
    
    AL_CHECK_ERROR(); // 초기화 후 오류 체크
    
    // 리스너 기본 설정 (위치: 0,0,0), (정면, 상향 Y축)
    setListenerPosition(0.0f, 0.0f, 0.0f);
    setListenerOrientation(0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

    std::cout << "OpenAL initialized successfully." << std::endl;
    return true;
}

void SoundManager::quitAL() {
    // OpenAL 컨텍스트가 유효한 경우에만 OpenAL 함수 호출
    if (context_ && device_) {
        alcMakeContextCurrent(context_); // 컨텍스트를 현재 스레드에 바인딩
        AL_CHECK_ERROR();

        // 모든 재생 중인 사운드 중지
        stopAllSounds();

        // 모든 소스 삭제
        for (ALuint source : availableSources_) {
            if (alIsSource(source)) { // 유효한 소스인지 확인
                alDeleteSources(1, &source);
            }
        }
        availableSources_.clear();
        playingSources_.clear();

        // 모든 버퍼 삭제
        for (auto const& [id, buffer] : soundBuffers_) {
            if (alIsBuffer(buffer)) { // 유효한 버퍼인지 확인
                alDeleteBuffers(1, &buffer);
            }
        }
        soundBuffers_.clear();

        alcMakeContextCurrent(NULL); // 컨텍스트 해제
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
        std::cerr << "OpenAL Error in " << filename << " (" << line << "): " << error << std::endl;
    }
}

bool SoundManager::loadSound(const std::string& filePath) {
    if (soundBuffers_.count(filePath)) {
        std::cerr << "Sound with ID '" << filePath << "' already loaded." << std::endl;
        return false;
    }

    ALuint buffer;
    alGenBuffers(1, &buffer);
    AL_CHECK_ERROR();

    std::filesystem::path path(filePath);
    std::string extension = path.extension().string();
    
    bool loaded = false;
    if (extension == ".wav" || extension == ".WAV") {
        loaded = loadWav(filePath, buffer);
    } else if (extension == ".mp3" || extension == ".MP3") {
        loaded = loadMp3(filePath, buffer);
    } else if (extension == ".ogg" || extension == ".OGG") {
        loaded = loadOgg(filePath, buffer);
    } else {
        std::cerr << "Unsupported sound file format: " << extension << std::endl;
        alDeleteBuffers(1, &buffer);
        return false;
    }

    if (loaded) {
        soundBuffers_[filePath] = buffer;
        std::cout << "Sound '" << filePath << "' loaded from " << filePath << std::endl;
        return true;
    } else {
        alDeleteBuffers(1, &buffer);
        return false;
    }
}

ALuint SoundManager::playSound(const std::string& filePath, float volume, float pitch, bool loop) {
    auto it = soundBuffers_.find(filePath);
    if (it == soundBuffers_.end()) {
        std::cerr << "Sound with ID '" << filePath << "' not found." << std::endl;
        return 0;
    }

    ALuint source = getFreeSource();
    if (source == 0) {
        std::cerr << "No free OpenAL sources available." << std::endl;
        return 0;
    }

    alSourcei(source, AL_BUFFER, it->second);
    alSourcef(source, AL_GAIN, volume);
    alSourcef(source, AL_PITCH, pitch);
    alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    AL_CHECK_ERROR();

    alSourcePlay(source);
    AL_CHECK_ERROR();

    playingSources_[source] = filePath; // 재생 중인 소스 목록에 추가
    return source;
}

void SoundManager::stopSound(ALuint sourceId) {
    if (playingSources_.count(sourceId)) {
        alSourceStop(sourceId);
        AL_CHECK_ERROR();
        returnSource(sourceId);
    }
}

void SoundManager::stopAllSounds() {
    for (auto const& [sourceId, filePath] : playingSources_) {
        alSourceStop(sourceId);
        AL_CHECK_ERROR();
    }
    // 모든 소스를 풀로 반환
    std::vector<ALuint> sourcesToReturn;
    for (auto const& [sourceId, filePath] : playingSources_) {
        sourcesToReturn.push_back(sourceId);
    }
    for (ALuint sourceId : sourcesToReturn) {
        returnSource(sourceId);
    }
    playingSources_.clear();
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
    alSource3f(sourceId, AL_POSITION, x, y, z);
    AL_CHECK_ERROR();
}

void SoundManager::setSourceVolume(ALuint sourceId, float volume) {
    alSourcef(sourceId, AL_GAIN, volume);
    AL_CHECK_ERROR();
}

void SoundManager::setSourcePitch(ALuint sourceId, float pitch) {
    alSourcef(sourceId, AL_PITCH, pitch);
    AL_CHECK_ERROR();
}

ALuint SoundManager::getFreeSource() {
    // 사용 가능한 소스 풀에서 가져오기
    if (!availableSources_.empty()) {
        ALuint source = availableSources_.back();
        availableSources_.pop_back();
        return source;
    }

    // 풀에 없으면 새로 생성
    ALuint newSource;
    alGenSources(1, &newSource);
    AL_CHECK_ERROR();
    if (newSource != 0) {
        return newSource;
    }
    return 0;
}

void SoundManager::returnSource(ALuint source) {
    // 소스 상태 초기화
    alSourceStop(source);
    alSourcei(source, AL_BUFFER, 0); // 버퍼 연결 해제
    alSourcef(source, AL_GAIN, 1.0f);
    alSourcef(source, AL_PITCH, 1.0f);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    AL_CHECK_ERROR();

    availableSources_.push_back(source);
    playingSources_.erase(source); // 재생 중인 목록에서 제거
}

// WAV 파일 로딩
bool SoundManager::loadWav(const std::string& filePath, ALuint buffer) {
    unsigned int channels;
    unsigned int sampleRate;
    drwav_uint64 totalPcmFrameCount;
    short* pData = drwav_open_file_and_read_pcm_frames_s16(filePath.c_str(), &channels, &sampleRate, &totalPcmFrameCount, NULL);
    if (pData == NULL) {
        std::cerr << "Failed to load WAV file: " << filePath << std::endl;
        return false;
    }

    ALenum alFormat;
    if (channels == 1) {
        alFormat = AL_FORMAT_MONO16;
    } else if (channels == 2) {
        alFormat = AL_FORMAT_STEREO16;
    } else {
        std::cerr << "Unsupported WAV channel count: " << channels << "\n";
        drwav_free(pData, NULL);
        return false;
    }

    alBufferData(buffer, alFormat, pData, totalPcmFrameCount * channels * sizeof(short), sampleRate);
    AL_CHECK_ERROR();
    drwav_free(pData, NULL);
    return true;
}

// MP3 파일 로딩
bool SoundManager::loadMp3(const std::string& filePath, ALuint buffer) {
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
        alFormat = AL_FORMAT_STEREO16;
    } else {
        std::cerr << "Unsupported MP3 channel count: " << config.channels << "\n";
        return false;
    }

    alBufferData(buffer, alFormat, pcm16.data(), pcm16.size() * sizeof(short), config.sampleRate);
    AL_CHECK_ERROR();
    return true;
}

// OGG 파일 로딩
bool SoundManager::loadOgg(const std::string& filePath, ALuint buffer) {
    int channels;
    int sample_rate;
    short* pcm;
    int samples = stb_vorbis_decode_filename(filePath.c_str(), &channels, &sample_rate, &pcm);

    if (samples == -1) {
        std::cerr << "Failed to load OGG file: " << filePath << "\n";
        return false;
    }

    ALenum alFormat;
    if (channels == 1) {
        alFormat = AL_FORMAT_MONO16;
    } else if (channels == 2) {
        alFormat = AL_FORMAT_STEREO16;
    } else {
        std::cerr << "Unsupported OGG channel count: " << channels << "\n";
        free(pcm);
        return false;
    }

    alBufferData(buffer, alFormat, pcm, samples * channels * sizeof(short), sample_rate);
    AL_CHECK_ERROR();
    free(pcm); // STB_VORBIS_free 대신 free 사용
    return true;
}