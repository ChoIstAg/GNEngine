#include "engine/resource/Animation.h"

/*
 * @brief Animation 생성자.
 * @param texturePath - 애니메이션에 사용될 텍스처(스프라이트 시트)의 파일 경로.
 * @param loop - 애니메이션의 반복 여부 (기본값: true).
 */
Animation::Animation(std::filesystem::path texturePath, bool loop)
    : texturePath_(std::move(texturePath)), loop_(loop) {}

/*
 * @brief 애니메이션에 새 프레임을 추가함.
 * @param frameRect - 스프라이트 시트에서 해당 프레임이 차지하는 사각형 영역.
 * @param duration - 해당 프레임의 지속 시간 (밀리초 단위).
 */
void Animation::addFrame(SDL_Rect frameRect, int duration) {
    frames_.push_back(frameRect);
    frameDurations_.push_back(duration);
}

/*
 * @brief 특정 인덱스의 프레임 사각형 영역을 반환함.
 * @param frameIndex - 가져올 프레임의 인덱스.
 * @return 해당 프레임의 SDL_Rect. 인덱스가 유효하지 않으면 빈 SDL_Rect를 반환.
 */
const SDL_Rect& Animation::getFrame(int frameIndex) const {
    if (frameIndex >= 0 && frameIndex < frames_.size()) {
        return frames_[frameIndex];
    }
    static const SDL_Rect emptyRect = {0, 0, 0, 0};
    return emptyRect;
}

/*
 * @brief 특정 인덱스의 프레임 지속 시간을 반환함.
 * @param frameIndex - 가져올 프레임의 인덱스.
 * @return 해당 프레임의 지속 시간 (밀리초). 인덱스가 유효하지 않으면 0을 반환.
 */
int Animation::getFrameDuration(int frameIndex) const {
    if (frameIndex >= 0 && frameIndex < frameDurations_.size()) {
        return frameDurations_[frameIndex];
    }
    return 0;
}

/*
 * @brief 전체 프레임 수를 반환함.
 * @return 애니메이션의 총 프레임 수.
 */
int Animation::getFrameCount() const {
    return frames_.size();
}

/*
 * @brief 애니메이션에 사용되는 텍스처의 파일 경로를 반환함.
 * @return 텍스처의 파일 경로.
 */
const std::filesystem::path& Animation::getTexturePath() const {
    return texturePath_;
}

/*
 * @brief 애니메이션의 반복 여부를 반환함.
 * @return 반복되면 true, 아니면 false.
 */
bool Animation::isLooping() const {
    return loop_;
}
