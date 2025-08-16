#pragma once
#include "GNEngine_API.h"
#include "flatbuffers/flatbuffers.h"

#include <string>
#include <vector>
#include <filesystem>

/* for flatBuffers */
#include "engine/flatbuffers_generated/definitions_generated.h"

namespace GNEngine {
    namespace data {
        struct Setting;
        struct Log;
    }
}

class GNEngine_API FileManager {
public:
    FileManager();

    // Settings
    void setSetting(const std::string& key, const std::string& value);
    std::string getSetting(const std::string& key, const std::string& defaultValue = "");
    void saveSettings(const std::filesystem::path& filePath);
    void loadSettings(const std::filesystem::path& filePath);

    // Logs
    void addLog(const std::string& message);
    void saveLogs(const std::filesystem::path& filePath);

private:
    flatbuffers::FlatBufferBuilder builder_;

    std::vector<flatbuffers::Offset<GNEngine::data::Setting>> settings_;
    std::vector<flatbuffers::Offset<GNEngine::data::Log>> logs_;
};