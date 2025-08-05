#pragma once

#include <cstdint>
#include <bitset>

// ECS에서 엔티티를 식별하는 고유 ID
using EntityId = uint32_t;

// 유효하지 않은 엔티티 ID를 나타내는 값
const EntityId INVALID_ENTITY_ID = 0;

// 최대 컴포넌트 수 (필요에 따라 조정)
const size_t MAX_COMPONENTS = 32;

// 엔티티의 컴포넌트 구성을 나타내는 비트셋
using Signature = std::bitset<MAX_COMPONENTS>;
