#pragma once
#include "../../GNEngine_API.h"

#include <string>
#include <filesystem>
#include <unordered_map>
#include <memory>

#include "GNEngine/resource/Animation.h"

/*
 * @class AnimationManager
 * @brief 애니메이션 데이터를 관리하는 클래스임.
 * JSON 파일을 파싱하여 Animation 객체를 생성하고 캐시에 저장하는 역할을 함.
 * 이 클래스는 애니메이션 데이터의 로딩과 접근을 담당하며, 실제 렌더링이나 상태 관리는 하지 않음.
 */ 
class GNEngine_API AnimationManager {
public:
    AnimationManager() = default;
    ~AnimationManager() = default;

    /*
     * @brief JSON 파일을 로드하여 애니메이션 데이터를 파싱하고 캐시에 저장함.
     * @param jsonPath - 애니메이션 데이터가 정의된 JSON 파일의 경로.
     * @return 로딩 및 파싱 성공 시 true, 실패 시 false.
     */
    bool loadAnimation(const std::filesystem::path& jsonPath);

    /*
     * @brief 캐시에서 애니메이션 데이터를 가져옴.
     * @param animationName - 가져올 애니메이션의 이름 (JSON 파일에 정의된 키).
     * @return 해당 이름의 std::shared_ptr<Animation>. 찾지 못하면 nullptr를 반환.
     */
    std::shared_ptr<Animation> getAnimation(const std::string& animationName);

private:
    std::unordered_map<std::string, std::shared_ptr<Animation>> animationCache_;
};

