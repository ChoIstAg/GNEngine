#pragma once

/*
 * TransformComponent는 게임 오브젝트의 2D 공간에서의
 * 위치, 회전, 크기를 나타내는 데이터 집합임.
 * 이 컴포넌트는 순수 데이터(POD) 컨테이너 역할을 함.
 */
#include "Component.h"

struct TransformComponent : public Component {

    /*
     * @brief TransformComponent의 생성자.
     * @param positionX, positionY 위치 X, Y
     * @param scaleX, scaleY 크기 배율 (너비, 높이)
     * @param rotatedAngle 회전된 각도
    */
    TransformComponent(float positionX = 0.0f, float positionY = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, float rotatedAngle = 0.0f) 
        : positionX_(positionX), positionY_(positionY), scaleX_(scaleX), scaleY_(scaleY), rotatedAngle_(rotatedAngle) {}

    float positionX_;
    float positionY_;

    /* 크기. (배수) */
    float scaleX_;
    float scaleY_;

    /* 0 ~ 360도(degree) 값 */
    float rotatedAngle_;
};                                                             
