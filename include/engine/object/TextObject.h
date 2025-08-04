#pragma once

#include "engine/ecs/Entity.h"
#include "engine/ecs/EntityManager.h"
#include "engine/component/TextComponent.h"
#include "engine/component/TransformComponent.h"
#include "engine/manager/resource/TextManager.h"
#include <memory>

// TextObject는 더 이상 Object를 상속받지 않고, 엔티티를 생성하고 컴포넌트를 추가하는 헬퍼 함수 역할을 합니다.
// 또는 특정 엔티티의 컴포넌트 조합을 정의하는 구조체로 사용될 수 있습니다.

class TextObject {
public:
    TextObject(EntityManager& entityManager, TextManager& textManager, std::unique_ptr<Text> text, float x, float y);

    EntityId getEntityId() const { return entityId_; }

private:
    EntityId entityId_;
};
