#pragma once
#include "../../GNEngine_API.h"

#include <memory>

#include "GNEngine/core/Component.h"
#include "GNEngine/resource/Animation.h"

/*
 * @class AnimationComponent
 * @brief 게임 오브젝트에 애니메이션 기능을 부여하는 컴포넌트.
 * @param animation 이 컴포넌트가 재생할 Animation 데이터.
 * @param playOnAwake(true) 컴포넌트 생성 시 바로 애니메이션을 재생할지 여부.
 */ 
class GNEngine_API AnimationComponent : public Component {
public:
    AnimationComponent(std::shared_ptr<Animation> animation, bool playOnAwake = true);

    /*
     * @brief 컴포넌트 업데이트 로직.
     * deltaTime을 사용하여 애니메이션 프레임을 업데이트함.
     */
    

    /*
     * @brief 애니메이션 재생을 시작함.
     */
    void play();

    /*
     * @brief 애니메이션 재생을 일시 정지함.
     */
    void pause();

    /*
     * @brief 애니메이션 재생을 중지하고 첫 프레임으로 되돌림.
     */
    void stop();

    /*
     * @brief 현재 재생 중인 프레임의 스프라이트 시트 내 사각형 영역을 반환함.
     * RenderComponent에서 이 정보를 사용하여 올바른 프레임을 렌더링함.
     * @return 현재 프레임의 SDL_Rect.
     */
    const SDL_Rect& getCurrentFrameRect() const;

    std::shared_ptr<Animation> getAnimation() const {
        return animation_;
    }

    /*
     * @brief 현재 애니메이션이 재생 중인지 여부를 반환함.
     * @return 재생 중이면 true, 아니면 false.
     */
    bool isPlaying() const { return isPlaying_; }

    /*
     * @brief 현재 애니메이션이 완료되었는지 여부를 반환함.
     * 반복 애니메이션이 아니면서 마지막 프레임까지 재생이 완료되었을 때 true를 반환함.
     * @return 애니메이션이 완료되었으면 true, 아니면 false.
     */
    bool isFinished() const { return isFinished_; }

public:
    std::shared_ptr<Animation> animation_;
    int currentFrame_;
    float frameTimer_;
    bool isPlaying_;
    bool isFinished_;
};

