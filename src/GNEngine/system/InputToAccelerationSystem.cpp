#include "GNEngine/system/InputToAccelerationSystem.h"
#include "GNEngine/component/InputControlComponent.h"

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
    // SDL_Log("InputToAccSystem: ActionEvent '%s' received for entity %u.", event.actionName.c_str(), event.targetEntityId);
    
    auto accelArray = entityManager_.getComponentArray<AccelerationComponent>();
    if (!accelArray || !accelArray->hasComponent(event.targetEntityId)) {
        // SDL_Log("InputToAccSystem: Entity %u does not have AccelerationComponent or array not found.", event.targetEntityId);
        return;
    }

    const size_t i = accelArray->getEntityToIndexMap().at(event.targetEntityId);
    const float ACCELERATION_VALUE = 200.0f; // 예시 가속도 값

    // 액션 이름에 따라 가속도를 직접 설정
    if (event.actionName == "move_forward") {
        accelArray->ay[i] = -ACCELERATION_VALUE;
    } else if (event.actionName == "move_backward") {
        accelArray->ay[i] = ACCELERATION_VALUE;
    } else if (event.actionName == "move_left") {
        accelArray->ax[i] = -ACCELERATION_VALUE;
    } else if (event.actionName == "move_right") {
        accelArray->ax[i] = ACCELERATION_VALUE;
    }
    // SDL_Log("InputToAccSystem: Entity %u acceleration set to (%.2f, %.2f)", event.targetEntityId, accelArray->ax[i], accelArray->ay[i]);
}
