#pragma once

#include "engine/core/Component.h"
#include "engine/core/Entity.h"

struct CameraComponent : public Component {
    float x, y;
    float zoom;
    EntityId targetEntityId; // 카메라가 따라갈 엔티티 ID

    CameraComponent(float x = 0.0f, float y = 0.0f, float zoom = 1.0f, EntityId targetId = INVALID_ENTITY_ID)
        : x(x), y(y), zoom(zoom), targetEntityId(targetId) {}
};
