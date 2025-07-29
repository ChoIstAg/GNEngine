#pragma once

/*
 * @brief 모든 컴포넌트의 기반이 되는 인터페이스.
 * 컴포넌트는 엔티티에 추가될 수 있는 데이터와 행위의 모듈식 블록을 나타냅니다.
*/
struct Component {
    // 가상 소멸자를 통해 다형적 삭제를 보장
    virtual ~Component() = default;
};