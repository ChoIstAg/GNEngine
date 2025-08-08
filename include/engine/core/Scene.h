#pragma once
#include "GNEngine_API.h"

#include <SDL3/SDL.h>

// 순환참조를 방지하기 위해 전방 선언함.
class EventManager;
class RenderManager;
class TextureManager;
class SoundManager;
class EntityManager;


/**
 * @brief Scene 클래스는 게임 씬의 기본 인터페이스를 정의함.
 * 각 씬은 이 클래스를 상속받아 onEnter, onExit, handleEvent, update, render 메서드를 구현해야 함.
 */ 
class GNEngine_API Scene {
protected:
    // 각 씬이 매니저에 접근할 수 있도록 참조를 저장
    EventManager& eventManager_;
    RenderManager& renderManager_;
    TextureManager& textureManager_;
    SoundManager& soundManager_;
    EntityManager& entityManager_;

public:
    Scene(EventManager& eventManager, RenderManager& renderManager, TextureManager& textureManager, SoundManager& soundManager, EntityManager& entityManager)
        : eventManager_(eventManager), renderManager_(renderManager), textureManager_(textureManager), soundManager_(soundManager), entityManager_(entityManager) {}
    virtual ~Scene() = default;
    
    // 씬 진입 시 호출.
    virtual void onEnter() = 0;
    virtual void onExit() = 0;       // 씬 종료 시 호출
    virtual void handleEvent(const SDL_Event& event) = 0; // 입력 처리
    virtual void update(float deltaTime) = 0;         // 논리 업데이트
    virtual void render(SDL_Renderer* renderer) = 0;  // 렌더링
};