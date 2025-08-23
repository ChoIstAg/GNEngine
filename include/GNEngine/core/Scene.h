#pragma once
#include "../../GNEngine_API.h"

/**
 * @brief Scene 클래스는 게임 씬의 기본 인터페이스를 정의함.
 * 각 씬은 이 클래스를 상속받아 onEnter, onExit, update 메서드를 구현해야 함.
 * 이 클래스는 어떤 Manager에도 의존하지 않으며, 순수한 인터페이스 역할만 함.
 */
class GNEngine_API Scene {
public:
    virtual ~Scene() = default;

    // 씬 진입 시 호출.
    virtual void onEnter() = 0;
    // 씬 종료 시 호출
    virtual void onExit() = 0;
    // 씬 로드
    virtual bool loadScene() { return true; }
    // 논리 업데이트
    virtual void update(float deltaTime) = 0;

protected:
    bool isLoaded_ = false;
};
