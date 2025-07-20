#pragma once

#include <map>
#include <typeindex>
#include <functional>
#include "EventInterface.h"
#include "EventManager.h"

class EventListenerComponent {
private:
    EventManager& eventManager_;
    std::map<std::type_index, EventManager::SubscriptionId> subscriptionIds_;
public:
    explicit EventListenerComponent(EventManager& eventManager) : eventManager_(eventManager) {}

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
