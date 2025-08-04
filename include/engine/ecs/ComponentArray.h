#pragma once

#include "Entity.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>

// 모든 컴포넌트 배열의 기본 인터페이스
class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(EntityId entity) = 0;
};

// 특정 타입 T의 컴포넌트를 저장하는 배열
template<typename T>
class ComponentArray : public IComponentArray {
public:
    void addComponent(EntityId entity, T&& component) { // R-value reference로 변경
        if (entityToIndexMap_.count(entity)) {
            throw std::runtime_error("ComponentArray: Component already added to entity.");
        }

        size_t newIndex = componentVector_.size();
        entityToIndexMap_[entity] = newIndex;
        indexToEntityMap_[newIndex] = entity;
        componentVector_.push_back(std::move(component)); // std::move 사용
    }

    void removeComponent(EntityId entity) {
        if (!entityToIndexMap_.count(entity)) {
            throw std::runtime_error("ComponentArray: Component not found for entity.");
        }

        // 제거할 컴포넌트의 인덱스
        size_t indexOfRemoved = entityToIndexMap_[entity];
        // 벡터의 마지막 컴포넌트의 인덱스
        size_t indexOfLast = componentVector_.size() - 1;

        // 제거할 컴포넌트 위치에 마지막 컴포넌트를 이동
        componentVector_[indexOfRemoved] = std::move(componentVector_[indexOfLast]); // std::move 사용

        // 매핑 업데이트
        EntityId entityOfLast = indexToEntityMap_[indexOfLast];
        entityToIndexMap_[entityOfLast] = indexOfRemoved;
        indexToEntityMap_[indexOfRemoved] = entityOfLast;

        // 벡터에서 마지막 요소 제거
        componentVector_.pop_back();

        // 매핑에서 제거
        entityToIndexMap_.erase(entity);
        indexToEntityMap_.erase(indexOfLast);
    }

    T& getComponent(EntityId entity) {
        if (!entityToIndexMap_.count(entity)) {
            throw std::runtime_error("ComponentArray: Component not found for entity.");
        }
        return componentVector_[entityToIndexMap_[entity]];
    }

    bool hasComponent(EntityId entity) const {
        return entityToIndexMap_.count(entity);
    }

    void entityDestroyed(EntityId entity) override {
        if (entityToIndexMap_.count(entity)) {
            removeComponent(entity); // 엔티티 파괴 시 해당 컴포넌트 제거
        }
    }

private:
    std::vector<T> componentVector_; // 실제 컴포넌트 데이터
    std::unordered_map<EntityId, size_t> entityToIndexMap_; // 엔티티 ID -> 벡터 인덱스
    std::unordered_map<size_t, EntityId> indexToEntityMap_; // 벡터 인덱스 -> 엔티티 ID
};
