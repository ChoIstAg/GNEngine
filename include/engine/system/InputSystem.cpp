#include "InputSystem.h"
#include "config.h"
#include "engine/component/InputControlComponent.h" // InputControlComponent.h 추가

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
    // InputControlComponent를 가진 모든 엔티티를 순회하며 입력 상태 업데이트
    // 현재는 첫 번째 InputControlComponent를 가진 엔티티만 제어한다고 가정
    for (EntityId entity : entityManager_.getEntitiesWith<InputControlComponent>()) {
        InputControlComponent* inputControl = entityManager_.getComponent<InputControlComponent>(entity);

        // Reset desired movement to 0
        inputControl->desiredX = 0.0f;
        inputControl->desiredY = 0.0f;

        for (const auto& keyInfo : event.heldKeys) {
            switch (keyInfo.scancode) {
                case SDL_SCANCODE_W:
                    inputControl->desiredY = -1.0f;
                    break;
                case SDL_SCANCODE_A:
                    inputControl->desiredX = -1.0f;
                    break;
                case SDL_SCANCODE_S:
                    inputControl->desiredY = 1.0f;
                    break;
                case SDL_SCANCODE_D:
                    inputControl->desiredX = 1.0f;
                    break;
                default:
                    break;
            }
        }
        // 첫 번째 엔티티만 제어하므로, 여기서 break
        break;
    }
}