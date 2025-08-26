#include "GNEngine/manager/FileManager.h"
#include "GNEngineRootPath.h"

#include <fstream>
#include <iostream>
#include <string_view>

#include <SDL3/SDL.h>


FileManager::FileManager() : builder_(1024), configFilePath_() {}



/*
 * @brief 설정 파일을 초기화하고 기본값을 설정하거나 기존 설정을 불러옴.
 * @param configFilePath: 설정 파일 경로
*/
void FileManager::init(const std::filesystem::path& configFilePath) {
    setConfigFilePath(configFilePath);
    
    if (configFilePath == "") { //configFilePath가 기본값일 경우.
        std::cerr << "[ERROR] FileManager - configFilePath is empty. \n";
        return;
    }

    if (!std::filesystem::exists(configFilePath_)) {
        // 상위 디렉토리가 없으면 생성
        std::filesystem::create_directories(configFilePath_.parent_path());

        // 모든 모니터의 데이터 추출 <- 나중에 다른 창 매니저나 렌더 매니저에 기능 이항 예정
        // int displayCount = 0; /* A number of display connected. */
        // SDL_DisplayID* displayIDs = SDL_GetDisplays(&displayCount);
        // if(!displayIDs){ SDL_Log("[ERROR] FileManager - Can't load DisplayIds : %d", SDL_GetError()); }
        // for(int i = 0; i < displayCount; i++) {
        //     SDL_DisplayID id = displayIDs[i];
        //     const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(id);
        // }

        // 유저의 주 모니터 크기를 기본 화면 크기로 설정. 
        SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
        if(!displayID) {
            SDL_Log("[ERROR] FileManager - Can't get display id. : %s", SDL_GetError());
        } else {
                const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(displayID);
                if(displayMode == nullptr) {
                    SDL_Log("[ERROR] FileManager - Can't get current display mode. : %s", SDL_GetError());
                } else {
                    setSetting("windowWidth", std::to_string(displayMode->w));
                    setSetting("windowHeight", std::to_string(displayMode->h));
                }
            }
        
        // 기본 설정 값 설정
        setSetting("fullscreen", "false");
        setSetting("masterVolume", "100");

        // 설정 저장
        saveSettings(configFilePath_);
        std::cout << "First Init! Created new config file: " << configFilePath_ << std::endl;
    } else {
        // 기존 설정 불러오기
        loadSettings(configFilePath_);
        std::cout << "Loaded existing config file: " << configFilePath_ << std::endl;
    }
}

/*
 * @brief 설정 값을 설정함. 내부 맵에 키-값으로 저장함.
 * @param key: 설정 키
 * @param value: 설정 값
*/
void FileManager::setSetting(std::string_view key, std::string_view value) 
{
    settings_map_[std::string(key)] = std::string(value);
}

/*
 * @brief 설정 값을 가져옴. 키가 존재하지 않으면 기본값을 반환함.
 * @param key: 설정 키
 * @param defaultValue: 키가 없을 때 반환할 기본값
 * @return 키에 해당하는 설정 값 또는 기본값
*/
std::string FileManager::getSetting(std::string_view key, std::string_view defaultValue) 
{
    if (settings_map_.count(std::string(key))) 
    {
        return settings_map_.at(std::string(key));
    }
    return std::string(defaultValue);
}

/*
 * @brief 현재 설정들을 FlatBuffers 형식으로 파일에 저장함.
 * @param filePath: 저장할 파일 경로
*/
void FileManager::saveSettings(const std::filesystem::path& filePath) 
{
    builder_.Clear();
    settings_offsets_.clear();

    for (const auto& pair : settings_map_) 
    {
        auto key = builder_.CreateString(pair.first);
        auto value = builder_.CreateString(pair.second);
        
        auto setting = GNEngine::data::CreateSetting(builder_, key, value);
        settings_offsets_.push_back(setting);
    }

    auto settings_vector = builder_.CreateVector(settings_offsets_);
    
    auto data = GNEngine::data::CreateData(builder_, settings_vector);
    builder_.Finish(data);

    std::ofstream outfile(filePath, std::ios::binary);
    if (outfile.is_open())
    {
        outfile.write(reinterpret_cast<const char*>(builder_.GetBufferPointer()), builder_.GetSize());
    }
}

/*
 * @brief 파일에서 FlatBuffers 형식의 설정을 불러와 맵에 채움.
 * @param filePath: 불러올 파일 경로
*/
void FileManager::loadSettings(const std::filesystem::path& filePath) 
{
    std::ifstream infile(filePath, std::ios::binary | std::ios::ate);
    if (!infile.is_open()) 
    {
        return;
    }

    auto size = infile.tellg();
    infile.seekg(0, std::ios::beg);
    
    std::vector<char> buffer(size);
    infile.read(buffer.data(), size);

    auto verifier = flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(buffer.data()), buffer.size());
    if (!GNEngine::data::VerifyDataBuffer(verifier)) {
        return;
    }

    auto data = GNEngine::data::GetData(buffer.data());
    auto settings = data->settings();
    
    settings_map_.clear();
    if (settings) 
    {
        for (const auto* setting : *settings) 
        {
            if (setting->key() && setting->value())
            {
                settings_map_[setting->key()->str()] = setting->value()->str();
            }
        }
    }
}

/*
 * @brief 로그 메시지를 추가함. 현재 타임스탬프와 함께 로그 오프셋 벡터에 저장됨.
 * @param message: 로그 메시지
*/
void FileManager::addLog(std::string_view message) 
{
    auto message_offset = builder_.CreateString(message);
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    auto log = GNEngine::data::CreateLog(builder_, timestamp, message_offset);
    logs_offsets_.push_back(log);
}

/*
 * @brief 저장된 로그들을 FlatBuffers 형식으로 파일에 저장함.
 * @param filePath: 저장할 파일 경로
*/
void FileManager::saveLogs(const std::filesystem::path& filePath) 
{
    builder_.Clear();

    auto logs_vector = builder_.CreateVector(logs_offsets_);
    
    auto data = GNEngine::data::CreateData(builder_, 0, logs_vector);
    builder_.Finish(data);

    std::ofstream outfile(filePath, std::ios::binary);
    if (outfile.is_open())
    {
        outfile.write(reinterpret_cast<const char*>(builder_.GetBufferPointer()), builder_.GetSize());
    }
    
    logs_offsets_.clear();
}



