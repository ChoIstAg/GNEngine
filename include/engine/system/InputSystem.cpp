#include "InputSystem.h"
#include "config.h"
#include "engine/component/InputControlComponent.h"
#include "engine/core/Entity.h" // EntityId를 사용하기 위해 추가

InputSystem::InputSystem(EventManager& eventManager, EntityManager& entityManager) // entityManager를 인자로 받음
    : eventManager_(eventManager)
{
    keysHeldSubId_ = eventManager_.subscribe<KeysHeldEvent>(
        // 람다 함수가 entityManager를 캡처하도록 수정
        [this, &entityManager](const KeysHeldEvent& event) { 
            onKeysHeld(event, entityManager); 
        }
    );
}

InputSystem::~InputSystem() {
    eventManager_.unsubscribe(typeid(KeysHeldEvent), keysHeldSubId_);
}

void InputSystem::onKeysHeld(const KeysHeldEvent& event, EntityManager& entityManager) {
    // InputControlComponent를 가진 모든 엔티티를 순회
    for (EntityId entity : entityManager.getEntitiesWith<InputControlComponent>()) {
        const auto* inputControl = entityManager.getComponent<InputControlComponent>(entity);

        // 현재 눌린 키가 엔티티의 keyActions 맵에 있는지 확인
        for (const auto& keyInfo : event.heldKeys) {
            auto it = inputControl->keyActions.find(keyInfo.scancode);
            if (it != inputControl->keyActions.end()) {
                // 매핑된 액션을 찾았다면 ActionEvent를 생성하여 dispatch
                eventManager_.dispatch(ActionEvent(entity, it->second));
            }
        }
    }
}
