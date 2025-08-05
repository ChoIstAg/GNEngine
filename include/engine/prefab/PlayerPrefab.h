#pragma once

#include "engine/core/Entity.h"

// 순환참조 방지
class EntityManager;
class EventManager;
class TextureManager;
class RenderManager;
class SoundManager;
class AnimationManager;

/*
* @class PlayerFactory
* @brief 플레이어 엔티티 생성을 위한 정적 팩토리 클래스임.
*
* 이 클래스는 플레이어 캐릭터에 필요한 모든 컴포넌트를 조합하여
* 하나의 완전한 엔티티를 생성하는 역할을 함.
* 객체를 생성하지 않고 정적 함수 create()를 통해 플레이어를 생성함.
*/
class PlayerPrefab {
public:
    /*
    * @brief 플레이어 엔티티를 생성하고 필요한 모든 컴포넌트를 추가함.
    * @param entityManager - 엔티티와 컴포넌트를 관리하는 EntityManager
    * @param eventManager - 이벤트 시스템을 관리하는 EventManager
    * @param textureManager - 텍스처 리소스를 관리하는 TextureManager
    * @param renderManager - 렌더링을 총괄하는 RenderManager
    * @param soundManager - 사운드 리소스를 관리하는 SoundManager
    * @param animationManager - 애니메이션 리소스를 관리하는 AnimationManager
    * @return 생성된 플레이어의 EntityId
    */
    static EntityId create(
        EntityManager& entityManager, 
        EventManager& eventManager, 
        TextureManager& textureManager, 
        RenderManager& renderManager, 
        SoundManager& soundManager, 
        AnimationManager& animationManager
    );
};
