#pragma once
#include "../../GNEngine_API.h"

/*
 * @brief 모든 컴포넌트의 기반이 되는 인터페이스.
 * 컴포넌트는 엔티티에 추가될 수 있는 데이터와 행위의 모듈식 블록을 나타냅니다.
*/ 
class GNEngine_API Component {
    // 가상 소멸자를 통해 다형적 소멸을 보장
    protected:
    virtual ~Component() = default;
};