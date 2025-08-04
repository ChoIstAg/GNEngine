#pragma once

#include "engine/ecs/Entity.h"
#include "engine/ecs/EntityManager.h"
#include "engine/component/TransformComponent.h"
#include "engine/component/RenderComponent.h"
#include "engine/component/SoundComponent.h"
#include "engine/component/PlayerAnimationControllerComponent.h"
#include "engine/component/PlayerMovementComponent.h"
#include "engine/manager/event/EventManager.h"
#include "engine/manager/render/RenderManager.h"
#include "engine/manager/sound/SoundManager.h"
#include "engine/manager/resource/TextureManager.h"
#include "engine/manager/resource/AnimationManager.h"

// TestObject는 더 이상 Object를 상속받지 않고, 엔티티를 생성하고 컴포넌트를 추가하는 헬퍼 함수 역할을 합니다.
// 또는 특정 엔티티의 컴포넌트 조합을 정의하는 구조체로 사용될 수 있습니다.

// 이 단계에서는 TestObject를 엔티티 생성 및 컴포넌트 추가를 위한 헬퍼 클래스로 간주합니다.
// TestObject의 생성자는 이제 EntityManager를 통해 엔티티를 생성하고 컴포넌트를 추가하는 역할을 합니다.

class TestObject {
public:
    // TestObject는 이제 엔티티를 생성하고 컴포넌트를 추가하는 헬퍼 클래스입니다.
    // 생성자에서 필요한 매니저와 EntityManager를 받아 엔티티를 구성합니다。
    TestObject(EntityManager& entityManager, EventManager& eventManager, TextureManager& textureManager, RenderManager& renderManager, SoundManager& soundManager, AnimationManager& animationManager);

    EntityId getEntityId() const { return entityId_; }

private:
    EntityId entityId_;
};