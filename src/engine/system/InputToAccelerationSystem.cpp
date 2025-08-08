#include "engine/system/InputToAccelerationSystem.h"
#include "engine/component/InputControlComponent.h"

InputToAccelerationSystem::InputToAccelerationSystem(EventManager& eventManager, EntityManager& entityManager)
    : eventManager_(eventManager), entityManager_(entityManager) {
    actionEventSubId_ = eventManager_.subscribe<ActionEvent>(
        [this](const ActionEvent& event) { onActionEvent(event); }
    );
}

InputToAccelerationSystem::~InputToAccelerationSystem() {
    eventManager_.unsubscribe(typeid(ActionEvent), actionEventSubId_);
}

void InputToAccelerationSystem::update(EntityManager& entityManager, float deltaTime) {
    // 이 함수는 더 이상 AccelerationComponent를 리셋하지 않습니다. 리셋 로직은 MovementSystem으로 이동했습니다.
}

void InputToAccelerationSystem::onActionEvent(const ActionEvent& event) {
    SDL_Log("InputToAccSystem: ActionEvent '%s' received for entity %u.", event.actionName.c_str(), event.targetEntityId);
    // 대상 엔티티가 AccelerationComponent를 가지고 있는지 확인
    if (!entityManager_.hasComponent<AccelerationComponent>(event.targetEntityId)) {
        SDL_Log("InputToAccSystem: Entity %u does not have AccelerationComponent.", event.targetEntityId);
        return;
    }

    auto* acceleration = entityManager_.getComponent<AccelerationComponent>(event.targetEntityId);
    const float ACCELERATION_VALUE = 200.0f; // 예시 가속도 값

    // 액션 이름에 따라 가속도를 직접 설정
    if (event.actionName == "move_forward") {
        acceleration->ay = -ACCELERATION_VALUE;
    } else if (event.actionName == "move_backward") {
        acceleration->ay = ACCELERATION_VALUE;
    } else if (event.actionName == "move_left") {
        acceleration->ax = -ACCELERATION_VALUE;
    } else if (event.actionName == "move_right") {
        acceleration->ax = ACCELERATION_VALUE;
    }
    SDL_Log("InputToAccSystem: Entity %u acceleration set to (%.2f, %.2f)", event.targetEntityId, acceleration->ax, acceleration->ay);
}