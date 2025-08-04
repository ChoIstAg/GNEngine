#pragma once

#include "Entity.h"
#include "ComponentArray.h"
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <stdexcept>
#include <queue>

/*
 * @class EntityManager
 * @brief ECS에서 엔티티와 컴포넌트의 관계를 관리하는 핵심 클래스임.
 *        엔티티 ID를 생성하고, 엔티티에 컴포넌트를 추가/제거/조회하는 기능을 제공함.
*/
class EntityManager {
public:
    EntityManager() = default;
    ~EntityManager() = default;

    // 복사 및 이동 금지
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
    EntityManager(EntityManager&&) = delete;
    EntityManager& operator=(EntityManager&&) = delete;

    /*
     * @brief 새로운 엔티티 ID를 생성하고 반환함.
     *        재활용 가능한 ID가 있으면 재활용하고, 없으면 새로운 ID를 생성함.
     * @return 생성된 엔티티의 고유 ID.
     */
    EntityId createEntity();

    /*
     * @brief 엔티티를 파괴하고 모든 관련 컴포넌트를 제거함.
     *        파괴된 엔티티의 ID는 재활용 풀에 추가됨.
     * @param entity - 파괴할 엔티티의 ID.
     */
    void destroyEntity(EntityId entity);

    /*
     * @brief 컴포넌트 타입을 등록하고 고유한 비트 인덱스를 할당함.
     * @tparam T - 등록할 컴포넌트의 타입.
     */
    template<typename T>
    void registerComponentType() {
        std::type_index type = typeid(T);
        if (componentTypes_.find(type) == componentTypes_.end()) {
            if (nextComponentType_ >= MAX_COMPONENTS) {
                throw std::runtime_error("EntityManager: Exceeded maximum number of component types.");
            }
            componentTypes_[type] = nextComponentType_++;
        }
    }

    /*
     * @brief 특정 엔티티에 컴포넌트를 추가함.
     * @tparam T - 추가할 컴포넌트의 타입.
     * @tparam Args - 컴포넌트 생성자에 전달할 인자들.
     * @param entity - 컴포넌트를 추가할 엔티티의 ID.
     * @param args - 컴포넌트 생성자에 전달할 인자들.
     * @return 추가된 컴포넌트의 참조.
     */
    template<typename T, typename... Args>
    T& addComponent(EntityId entity, Args&&... args) {
        std::type_index type = typeid(T);
        if (componentTypes_.find(type) == componentTypes_.end()) {
            throw std::runtime_error("EntityManager: Component type not registered. Call registerComponentType<T>() first.");
        }

        ComponentArray<T>* componentArray = getComponentArray<T>();
        if (!componentArray) {
            componentArrays_[type] = std::make_shared<ComponentArray<T>>();
            componentArray = static_cast<ComponentArray<T>*>(componentArrays_[type].get());
        }
        componentArray->addComponent(entity, std::move(T(std::forward<Args>(args)...)));

        // 엔티티 시그니처 업데이트
        entitySignatures_[entity].set(componentTypes_[type]);

        return componentArray->getComponent(entity);
    }

    /*
     * @brief 특정 엔티티의 컴포넌트를 조회함.
     * @tparam T - 조회할 컴포넌트의 타입.
     * @param entity - 컴포넌트를 조회할 엔티티의 ID.
     * @return 조회된 컴포넌트의 포인터 (없으면 nullptr).
     */
    template<typename T>
    T* getComponent(EntityId entity) {
        std::type_index type = typeid(T);
        if (componentTypes_.find(type) == componentTypes_.end()) {
            throw std::runtime_error("EntityManager: Component type not registered.");
        }

        ComponentArray<T>* componentArray = getComponentArray<T>();
        if (componentArray && componentArray->hasComponent(entity)) {
            return &componentArray->getComponent(entity);
        }
        return nullptr;
    }

    /*
     * @brief 특정 엔티티가 특정 컴포넌트를 가지고 있는지 확인함.
     * @tparam T - 확인할 컴포넌트의 타입.
     * @param entity - 확인할 엔티티의 ID.
     * @return 컴포넌트를 가지고 있으면 true, 아니면 false.
     */
    template<typename T>
    bool hasComponent(EntityId entity) {
        std::type_index type = typeid(T);
        if (componentTypes_.find(type) == componentTypes_.end()) {
            return false; // 등록되지 않은 컴포넌트 타입은 가지고 있지 않음
        }
        ComponentArray<T>* componentArray = getComponentArray<T>();
        return componentArray && componentArray->hasComponent(entity);
    }

    /*
     * @brief 특정 엔티티에서 컴포넌트를 제거함.
     * @tparam T - 제거할 컴포넌트의 타입.
     * @param entity - 컴포넌트를 제거할 엔티티의 ID.
     */
    template<typename T>
    void removeComponent(EntityId entity) {
        std::type_index type = typeid(T);
        if (componentTypes_.find(type) == componentTypes_.end()) {
            return; // 등록되지 않은 컴포넌트 타입은 제거할 필요 없음
        }

        ComponentArray<T>* componentArray = getComponentArray<T>();
        if (componentArray) {
            componentArray->removeComponent(entity);
            // 엔티티 시그니처 업데이트
            entitySignatures_[entity].reset(componentTypes_[type]);
        }
    }

    /*
     * @brief 특정 시그니처를 가진 모든 엔티티의 ID를 반환함.
     * @tparam Args - 쿼리할 컴포넌트 타입들.
     * @return 시그니처와 일치하는 엔티티 ID들의 벡터.
     */
    template<typename... Args>
    std::vector<EntityId> getEntitiesWith() {
        Signature requiredSignature;
        (registerComponentType<Args>(), ...); // 컴포넌트 타입이 등록되어 있는지 확인
        ((requiredSignature.set(componentTypes_[typeid(Args)])), ...);

        std::vector<EntityId> matchingEntities;
        for (EntityId entity : activeEntities_) {
            if ((entitySignatures_[entity] & requiredSignature) == requiredSignature) {
                matchingEntities.push_back(entity);
            }
        }
        return matchingEntities;
    }

    /*
     * @brief 모든 활성 엔티티의 ID를 반환함.
     * @return 활성 엔티티 ID들의 벡터.
     */
    std::vector<EntityId> getAllEntities() const;

private:
    EntityId nextEntityId_ = 1; // 0은 INVALID_ENTITY_ID로 사용
    std::vector<EntityId> activeEntities_; // 현재 활성 상태인 엔티티 ID 목록
    std::queue<EntityId> availableEntityIds_; // 재활용 가능한 엔티티 ID 풀
    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> componentArrays_; // 컴포넌트 타입별 배열
    std::unordered_map<std::type_index, size_t> componentTypes_; // 컴포넌트 타입 -> 비트 인덱스
    size_t nextComponentType_ = 0; // 다음 할당할 컴포넌트 비트 인덱스
    std::unordered_map<EntityId, Signature> entitySignatures_; // 엔티티 ID -> 시그니처

    template<typename T>
    ComponentArray<T>* getComponentArray() {
        auto it = componentArrays_.find(typeid(T));
        if (it != componentArrays_.end()) {
            return static_cast<ComponentArray<T>*>(it->second.get());
        }
        return nullptr;
    }
};
