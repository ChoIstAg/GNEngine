#pragma once
#include "GNEngine_API.h"
#include "flatbuffers/flatbuffers.h"

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <map>

// definitions_generated 파일은 이 명령어로 생성. 
//./lib/flatbuffers/flatc.exe -c -o "include/engine/flatbuffers_generated" "asset/text/fbs/definitions.fbs"
#include "GNEngine/flatbuffers_generated/definitions_generated.h"

namespace GNEngine {
    namespace data {
        struct Setting;
        struct Log;
    }
}

/* 
 * @brief 모든 로그와 파일 읽고 쓰기를 담당하는 매니저.
 * @note Using Singleton Pattern
*/
class GNEngine_API FileManager {
public:
    static FileManager& getInstance() {
        static FileManager instance;
        return instance;
    }

    // 복사 및 대입 삭제 (상글톤)
    FileManager(const FileManager&) = delete;
    void operator=(const FileManager&) = delete;

    // Settings
    void init();
    void setSetting(std::string_view key, std::string_view value);
    std::string getSetting(std::string_view key, std::string_view defaultValue = "");
    void saveSettings(const std::filesystem::path& filePath);
    void loadSettings(const std::filesystem::path& filePath);
    
    // Logs
    void addLog(std::string_view message);
    void saveLogs(const std::filesystem::path& filePath);

private:
    // Private for Singleton pattern
    FileManager(); 
    ~FileManager() {
        std::cerr << "FileManager instance is successfully destroyed.\n";
    }

    flatbuffers::FlatBufferBuilder builder_;

    /* @brief For saving data */
    std::vector<flatbuffers::Offset<GNEngine::data::Setting>> settings_offsets_;
    std::vector<flatbuffers::Offset<GNEngine::data::Log>> logs_offsets_;
    
    /* @brief For easy access */
    std::map<std::string, std::string> settings_map_;
};

