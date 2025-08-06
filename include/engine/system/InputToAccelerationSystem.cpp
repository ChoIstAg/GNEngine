#include "InputToAccelerationSystem.h"
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

void InputToAccelerationSystem::update(EntityManager& entityManager) {
    // 입력으로 제어되는 모든 엔티티의 가속도를 매 프레임 초기에 0으로 리셋함.
    for (auto entity : entityManager.getEntitiesWith<InputControlComponent, AccelerationComponent>()) {
        auto* acceleration = entityManager.getComponent<AccelerationComponent>(entity);
        acceleration->ax = 0.0f;
        acceleration->ay = 0.0f;
    }
}

void InputToAccelerationSystem::onActionEvent(const ActionEvent& event) {
    // 대상 엔티티가 AccelerationComponent를 가지고 있는지 확인
    if (!entityManager_.hasComponent<AccelerationComponent>(event.targetEntityId)) {
        return;
    }

    auto* acceleration = entityManager_.getComponent<AccelerationComponent>(event.targetEntityId);
    const float ACCELERATION_VALUE = 200.0f; // 예시 가속도 값

    // 액션 이름에 따라 가속도를 누적함
    if (event.actionName == "move_forward") {
        acceleration->ay -= ACCELERATION_VALUE;
    } else if (event.actionName == "move_backward") {
        acceleration->ay += ACCELERATION_VALUE;
    } else if (event.actionName == "move_left") {
        acceleration->ax -= ACCELERATION_VALUE;
    } else if (event.actionName == "move_right") {
        acceleration->ax += ACCELERATION_VALUE;
    }
}
