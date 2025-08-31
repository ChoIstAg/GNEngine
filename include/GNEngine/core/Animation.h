#pragma once
#include "../GNEngine_API.h" 

#include <vector>
#include <string>
#include <filesystem>
#include <SDL3/SDL_rect.h>

/*
 * @class Animation
 * @brief 애니메이션 데이터
 * 텍스처 경로, 각 프레임의 영역과 지속 시간, 반복 여부만 정의하고 Texture 자체는 갖고있지 않음.
 * 이 클래스는 데이터 컨테이너 역할을 하며, 실제 애니메이션 로직은 AnimationComponent에서 처리함.
 */
class GNEngine_API Animation {
public:
    /*
     * @brief Animation 생성자
     * @param texturePath - 애니메이션에 사용될 텍스처(스프라이트 시트) 파일 경로.
     * @param loop - 애니메이션의 반복 여부 (기본값 true).
     */
    Animation(std::filesystem::path texturePath, bool loop = true);

    /*
     * @brief 애니메이션에 새 프레임을 추가함.
     * @param frameRect - 스프라이트 시트에서 해당 프레임이 차지하는 사각형 영역.
     * @param duration - 해당 프레임의 지속 시간 (밀리초 단위).
     */
    void addFrame(SDL_Rect frameRect, int duration);

    /*
     * @brief 특정 인덱스의 프레임 사각형 영역을 반환함.
     * @param frameIndex - 가져올 프레임의 인덱스.
     * @return 해당 프레임의 SDL_Rect. 인덱스가 유효하지 않으면 빈 SDL_Rect를 반환.
     */
    const SDL_Rect& getFrame(int frameIndex) const;

    /*
     * @brief 특정 인덱스의 프레임 지속 시간을 반환함.
     * @param frameIndex - 가져올 프레임의 인덱스.
     * @return 해당 프레임의 지속 시간 (밀리초). 인덱스가 유효하지 않으면 0을 반환.
     */
    int getFrameDuration(int frameIndex) const;

    /*
     * @brief 전체 프레임 수를 반환함.
     * @return 애니메이션의 총 프레임 수.
     */
    int getFrameCount() const;

    /*
     * @brief 애니메이션에 사용되는 텍스처의 파일 경로를 반환함.
     * @return 텍스처의 파일 경로.
     */
    const std::filesystem::path& getTexturePath() const;

    /*
     * @brief 애니메이션의 반복 여부를 반환함.
     * @return 반복하면 true, 아니면 false.
     */
    bool isLooping() const;

private:
    std::filesystem::path texturePath_;
    std::vector<SDL_Rect> frames_;
    std::vector<int> frameDurations_;
    bool loop_;
};