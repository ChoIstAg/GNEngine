#pragma once

#include <vector>
#include <memory>
#include <typeindex>
#include <map>
#include <stdexcept>
#include "engine/component/Component.h"

class Object {
public:
    Object() = default;
    virtual ~Object() = default;

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

    virtual void update(float dt) {}
    virtual void render() {}

protected:
    std::map<std::type_index, std::unique_ptr<Component>> components_;
};
