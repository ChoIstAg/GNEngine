#pragma once
#include "../../GNEngine_API.h"

#include <map>
#include <typeindex>
#include <functional>
#include "engine/core/Component.h"
#include "engine/core/EventInterface.h"
#include "engine/manager/EventManager.h"

// This file is lagacy code that is now unused. 

/**
 * @class EventListenerComponent
 * @brief 이벤트를 구독하고 처리하는 컴포넌트.
 * 
 * 이 컴포넌트는 EventManager와 연결되어 있으며, 특정 이벤트 타입에 대한 콜백 함수를 등록하고,
 * 해당 이벤트가 발생했을 때 콜백을 호출함.
 * 
 * 구독 해지 시 모든 등록된 이벤트 리스너를 제거함.
 * @param owner 이 컴포넌트의 소유자 Entity.
 * @param eventManager 이벤트를 구독하고 관리할 EventManager 인스턴스.
 */
struct GNEngine_API EventListenerComponent : public Component {
private:
    EventManager& eventManager_;
    std::map<std::type_index, EventManager::SubscriptionId> subscriptionIds_;
public:
    explicit EventListenerComponent(Object& owner, EventManager& eventManager) : Component(owner), eventManager_(eventManager) {}

    ~EventListenerComponent() {
        if (!&eventManager_) return;
        for (const auto& pair : subscriptionIds_) {
            eventManager_.unsubscribe(pair.first, pair.second);
        }
    }

    // Disable copy and move semantics to prevent issues with subscription management.
    EventListenerComponent(const EventListenerComponent&) = delete;
    EventListenerComponent& operator=(const EventListenerComponent&) = delete;
    EventListenerComponent(EventListenerComponent&&) = delete;
    EventListenerComponent& operator=(EventListenerComponent&&) = delete;

    /* Add listener and subscribe Event*/
    template<typename T_Event>
    void addListener(std::function<void(const T_Event&)> callback) {
        if (!&eventManager_) return;
        
        std::type_index eventType = typeid(T_Event);
        
        // If a listener for this event type is already registered, remove it first.
        removeListener<T_Event>();

        // Subscribe to the event and store the subscription ID.
        int subId = eventManager_.subscribe<T_Event>(std::move(callback));
        subscriptionIds_[eventType] = subId;
    }

    template<typename T_Event>
    void removeListener() {
        if (!&eventManager_) { return; };

        std::type_index eventType = typeid(T_Event);
        auto it = subscriptionIds_.find(eventType);
        if (it != subscriptionIds_.end()) {
            eventManager_.unsubscribe(it->first, it->second);
            subscriptionIds_.erase(it);
        }
    }
};
