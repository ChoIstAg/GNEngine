#include "InputSystem.h"

InputSystem::InputSystem(EventManager& eventManager, EntityManager& entityManager)
    : eventManager_(eventManager), entityManager_(entityManager)
{
    keysHeldSubId_ = eventManager_.subscribe<KeysHeldEvent>(
        [this](const KeysHeldEvent& event) { onKeysHeld(event); }
    );
}

InputSystem::~InputSystem() {
    eventManager_.unsubscribe(typeid(KeysHeldEvent), keysHeldSubId_);
}

void InputSystem::update(EntityManager& entityManager, float deltaTime) {
    // InputSystem은 주로 이벤트를 통해 작동하므로, update에서는 특별히 할 일이 없을 수 있음.
    // 하지만, 필요하다면 여기서 지속적인 입력 상태를 처리할 수 있음.
}

void InputSystem::onKeysHeld(const KeysHeldEvent& event) {
    for (EntityId entity : entityManager_.getEntitiesWith<PlayerMovementComponent, VelocityComponent, AccelerationComponent>()) {
        VelocityComponent* velocity = entityManager_.getComponent<VelocityComponent>(entity);
        AccelerationComponent* acceleration = entityManager_.getComponent<AccelerationComponent>(entity);

        // Reset acceleration to simulate deceleration when no keys are pressed
        acceleration->ax = 0.0f;
        acceleration->ay = 0.0f;

        bool moving = false;
        for (const auto& keyInfo : event.heldKeys) {
            switch (keyInfo.scancode) {
                case SDL_SCANCODE_W:
                    acceleration->ay = -ACCELERATION_RATE;
                    moving = true;
                    break;
                case SDL_SCANCODE_A:
                    acceleration->ax = -ACCELERATION_RATE;
                    moving = true;
                    break;
                case SDL_SCANCODE_S:
                    acceleration->ay = ACCELERATION_RATE;
                    moving = true;
                    break;
                case SDL_SCANCODE_D:
                    acceleration->ax = ACCELERATION_RATE;
                    moving = true;
                    break;
                default:
                    break;
            }
        }

        // Apply deceleration if not moving
        if (!moving) {
            // Simple deceleration: reduce velocity towards zero
            if (velocity->vx > 0) velocity->vx = std::max(0.0f, velocity->vx - DECELERATION_RATE * 0.016f); // Assuming 60 FPS for now
            else if (velocity->vx < 0) velocity->vx = std::min(0.0f, velocity->vx + DECELERATION_RATE * 0.016f);
            if (velocity->vy > 0) velocity->vy = std::max(0.0f, velocity->vy - DECELERATION_RATE * 0.016f);
            else if (velocity->vy < 0) velocity->vy = std::min(0.0f, velocity->vy + DECELERATION_RATE * 0.016f);
        }

        // Clamp velocity to max speed
        float currentSpeed = std::sqrt(velocity->vx * velocity->vx + velocity->vy * velocity->vy);
        if (currentSpeed > MOVE_SPEED) {
            velocity->vx = (velocity->vx / currentSpeed) * MOVE_SPEED;
            velocity->vy = (velocity->vy / currentSpeed) * MOVE_SPEED;
        }
    }
}