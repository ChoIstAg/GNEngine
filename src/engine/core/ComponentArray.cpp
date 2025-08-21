#include "engine/core/ComponentArray.h"
#include "engine/manager/EntityManager.h"

// SoAComponentArray의 static 멤버 변수 정의
GNEngine_API std::unordered_map<EntityID, size_t> SoAComponentArray::entityToIndexMap;
GNEngine_API std::unordered_map<size_t, EntityID> SoAComponentArray::indexToEntityMap;

/*
 * @brief 엔티티가 파괴될 때 호출되어 공유 맵과 각 컴포넌트 배열의 데이터를 정리함.
*/
GNEngine_API void SoAComponentArray::entityDestroyed(EntityID entity) {
    if (!entityToIndexMap.count(entity)) {
        return; // 이 엔티티는 SoA 컴포넌트를 가지고 있지 않음
    }

    // 1. 제거할 엔티티의 인덱스와 마지막 요소의 인덱스를 찾음
    size_t indexOfRemoved = entityToIndexMap.at(entity);
    size_t indexOfLast = entityToIndexMap.size() - 1;

    if (indexOfRemoved == indexOfLast) {
        // 제거할 엔티티가 마지막 요소인 경우, 그냥 맵에서 제거
        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfRemoved);
    } else {
        // 마지막 요소를 제거된 엔티티의 위치로 이동
        EntityID lastEntity = indexToEntityMap.at(indexOfLast);
        entityToIndexMap[lastEntity] = indexOfRemoved;
        indexToEntityMap[indexOfRemoved] = lastEntity;

        // 맵에서 원래 엔티티와 마지막 인덱스 항목 제거
        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLast);
    }
    
    // TODO: EntityManager::destroyEntity에서 모든 SoA ComponentArray에 대해 
    //       직접 swapAndPop(indexOfRemoved, indexOfLast)를 호출하여 데이터 벡터를 재정렬하는 코드가 필요함.
}

