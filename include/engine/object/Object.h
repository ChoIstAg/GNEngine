#pragma once

#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <stdexcept>
#include "engine/component/Component.h"

/*
 * @brief 내 모든 오브젝트 개체의 기반 클래스.
 * @note 컴포넌트를 조합하여 동작과 데이터를 정의하는 역할을 함.
*/
class Object {
public:
    Object() = default;
    virtual ~Object() = default;

    // 컴포넌트의 소유권은 Object가 단독으로 가지므로 복사 금지.
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&&) = default;
    Object& operator=(Object&&) = default;

    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... args) {
        auto component = std::make_unique<T>(std::forward<TArgs>(args)...);
        T* ptr = component.get();
        components_[typeid(T)] = std::move(component);
        return *ptr;
    }

    template<typename T>
    T* getComponent() {
        auto it = components_.find(typeid(T));
        if (it != components_.end()) {
            // dynamic_cast로 안전한 타입 변환 보장.
            return dynamic_cast<T*>(it->second.get());
        }
        return nullptr;
    }
    
    template<typename T>
    const T* getComponent() const {
        auto it = components_.find(typeid(T));
        if (it != components_.end()) {
            return dynamic_cast<const T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    bool hasComponent() const {
        return components_.count(typeid(T)) > 0;
    }

    template<typename T>
    void removeComponent() {
        components_.erase(typeid(T));
    }

    // 파생 클래스에서 구현할 업데이트 및 렌더링 로직.
    virtual void update(float dt) {}
    virtual void render() {}

protected:

    std::unordered_map<std::type_index, std::unique_ptr<Component>> components_;
};