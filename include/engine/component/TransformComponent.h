#pragma once

/*
 * TransformComponent는 게임 오브젝트의 2D 공간에서의
 * 위치, 회전, 크기를 나타내는 데이터 집합임.
 * 이 컴포넌트는 순수 데이터(POD) 컨테이너 역할을 함.
 */
struct TransformComponent
{
    float positionX = 0.0f;
    float positionY = 0.0f;

    /* 크기. (배수) */
    float scaleX = 1.0f;
    float scaleY = 1.0f;

    /* 0 ~ 360도(degree) 값 */
    float rotation = 0.0f;
};                                                             
