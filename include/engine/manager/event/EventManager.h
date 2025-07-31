#pragma once

#include <functional>
#include <unordered_map>
#include <list> /* 리스트를 사용하는 이유: 오버헤드 크지만 순회, 값 찾기가 빨라서. */
#include <typeindex>
#include <any>
#include <utility> /* for std::move. */
#include <iostream>
#include "engine/event/EventInterface.h"


class EventManager {
public:
    using SubscriptionId = int;
    using Callback = std::function<void(const std::any&)>; /* 범용 함수 타입. (void 반환형) */

private:
    struct Subscriber {
        SubscriptionId id;
        Callback callback; /* 이벤트 발생시 실행 할 함수*/
    };
    
    /* key= 이벤트 타입(type_index. typeid()로 사용하기.) : value= 구독자 목록(list) */
    std::unordered_map<std::type_index, std::list<Subscriber>> subscribers_; 
    SubscriptionId nextId_ = 0; /* For allocation of a next subscripttion id. */

public:
    EventManager() 
        : nextId_(0) {
        subscribers_.clear();
    };
    
    ~EventManager() {
        subscribers_.clear(); /* 모든 구독자 목록을 비움. */
        std::cerr << "EventManager destroyed and all subscriptions cleared." << std::endl;
    }

    /* 구독하기 */
    template<typename T_Event>
    SubscriptionId subscribe (std::function<void(const T_Event&)> callback) {
        /* 래퍼 람다 객체로 타입을 소거함. ->  하나의 컨테이너(list)에 저장, 유연성 증가. */
        auto wrapper = [cb = std::move(callback)](const std::any& eventData) {
            if (const T_Event* event = std::any_cast<T_Event>(&eventData)) {
                cb(*event);
            }
        };
        
        SubscriptionId id = nextId_++; /* Assign next id into corrunt id and increase next id by 1 */
        subscribers_[typeid(T_Event)].push_back({id, wrapper});

        return id; /* 현재 이벤트 타입 id 반환. */
    }

    /* 구독 해지하기*/
    void unsubscribe (const std::type_index& eventType, SubscriptionId id) {
        if (subscribers_.count(eventType)) { /* 만약 이벤트를 구독한 리스너가 존재한다면 */
            auto& subscribers = subscribers_.at(eventType); 
            /* 현재 이벤트 id와 구독자의 이벤트 id가 일치하면 구독자를 목록에서 제거*/
            subscribers.remove_if( [&id](const auto& subscriber) { return (subscriber.id == id); });
        }
    }

    /* 이벤트 발생시 각 리스너의 함수 호출 */
    template<typename T_Event>
    void dispatch (const T_Event& event) {
        std::type_index eventType = typeid(T_Event);
        if (subscribers_.count(eventType)) { /* 해당 이벤트를 구독한 리스너가 존재한다면 */
            auto& subscribers = subscribers_.at(eventType); /* 구독자 list 자체를 참조 */
            for (const auto& sub : subscribers) {
                sub.callback(event); /* 리스너가 콜백 함수 실행 */
            }
        }
    }
    
};
