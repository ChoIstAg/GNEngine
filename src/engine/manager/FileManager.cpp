#include "engine/manager/FileManager.h"
#include "engine/flatbuffers_generated/definitions_generated.h"
#include <fstream>

FileManager::FileManager() {}

/*
 * 설정값 추가
*/
void FileManager::setSetting(const std::string& key, const std::string& value) {
    auto key_offset = builder_.CreateString(key);
    auto value_offset = builder_.CreateString(value);

    auto setting = GNEngine::data::CreateSetting(builder_, key_offset, value_offset);
    settings_.push_back(setting);
}

/*
 * 설정값 불러오기
*/
std::string FileManager::getSetting(const std::string& key, const std::string& defaultValue) {
    // This part will be implemented after loading settings is done
    return defaultValue;
}

/*
 * 설정값 저장
*/
void FileManager::saveSettings(const std::filesystem::path& filePath) {
    auto settings_vector = builder_.CreateVector(settings_);
    auto settings = GNEngine::data::CreateSettings(builder_, settings_vector);
    builder_.Finish(settings);

    uint8_t* buf = builder_.GetBufferPointer();
    int size = builder_.GetSize();

    std::ofstream ofs(filePath, std::ios::binary);
    ofs.write(reinterpret_cast<char*>(buf), size);
}

/*
 * 설정값 불러오기
*/
void FileManager::loadSettings(const std::filesystem::path& filePath) {
    std::ifstream ifs(filePath, std::ios::binary | std::ios::ate);
    if (!ifs.is_open()) {
        return;
    }

    auto size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!ifs.read(buffer.data(), size)) {
        return;
    }

    auto settings = GNEngine::data::GetSettings(buffer.data());
    
    for (const auto* setting : *settings->settings()) {
        // For now, we just print the loaded settings
        printf("Loaded setting: %s = %s\n", setting->key()->c_str(), setting->value()->c_str());
    }
}

/*
 * 로그 추가
*/
void FileManager::addLog(const std::string& message) {
    auto message_offset = builder_.CreateString(message);
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    auto log = GNEngine::data::CreateLog(builder_, timestamp, message_offset);
    logs_.push_back(log);
}

/*
 * 로그 저장
*/
void FileManager::saveLogs(const std::filesystem::path& filePath) {
    auto logs_vector = builder_.CreateVector(logs_);
    auto logs = GNEngine::data::CreateLogs(builder_, logs_vector);
    builder_.Finish(logs);

    uint8_t* buf = builder_.GetBufferPointer();
    int size = builder_.GetSize();

    std::ofstream ofs(filePath, std::ios::binary);
    ofs.write(reinterpret_cast<char*>(buf), size);
}