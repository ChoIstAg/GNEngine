#pragma once
#include <string>

/**
 * @brief 모든 이벤트의 기반이 되는 기본 구조체입니다.
 * 
 * 'handled' 플래그를 포함합니다. 리스너는 이벤트를 처리한 후 이 값을 true로 설정하여,
 * 동일한 이벤트를 구독하는 다른 리스너들이 해당 이벤트를 더 이상 처리하지 않도록 할 수 있습니다.
 * @note 이 'handled' 플래그가 올바르게 동작하려면 EventManager와 EventListenerComponent가
 * 이벤트를 수정 가능한 참조(non-const reference)로 전달해야 합니다.
 */
struct Event {
    bool handled = false;
};

// --- 테스트 ---
struct TestEvent : public Event {
    std::string message;
    TestEvent(const std::string_view message) : message(message) {}
};


// --- 창 관련 이벤트 ---

/**
 * @brief 창이 닫힐 때 발생하는 이벤트입니다.
 */
struct WindowCloseEvent : public Event {};

/**
 * @brief 창 크기가 조절될 때 발생하는 이벤트입니다.
 */
struct WindowResizeEvent : public Event {
    unsigned int width, height;

    WindowResizeEvent(unsigned int newWidth, unsigned int newHeight)
        : width(newWidth), height(newHeight) {}
};


// --- 키보드 관련 이벤트 ---

/**
 * @brief 키보드 이벤트의 기반이 되는 구조체입니다.
 */
struct KeyEvent : public Event {
    int keyCode; // 엔진 고유의 키 코드를 사용합니다.

protected:
    KeyEvent(int keycode) : keyCode(keycode) {}
};

/**
 * @brief 키가 눌렸을 때 발생하는 이벤트입니다.
 */
struct KeyPressedEvent : public KeyEvent {

    KeyPressedEvent(int keycode)
        : KeyEvent(keycode) {}
};

/**
 * @brief 키가 계속 눌려있는 상태일 때 발생하는 이벤트입니다.
 */
struct KeyRepeatEvent : public KeyEvent {
    int repeatCount; // 키가 계속 눌리고 있을 경우 반복 횟수

    KeyRepeatEvent(int keycode, int repeat)
        : KeyEvent(keycode), repeatCount(repeat) {}
};

/**
 * @brief 키에서 손을 뗐을 때 발생하는 이벤트입니다.
 */
struct KeyReleasedEvent : public KeyEvent {
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}
};


// --- 마우스 관련 이벤트 ---

/**
 * @brief 마우스 커서가 움직일 때 발생하는 이벤트입니다.
 */
struct MouseMovedEvent : public Event {
    float mouseX, mouseY;

    MouseMovedEvent(float x, float y) : mouseX(x), mouseY(y) {}
};

/**
 * @brief 마우스 휠을 스크롤할 때 발생하는 이벤트입니다.
 */
struct MouseScrolledEvent : public Event {
    float xOffset, yOffset;

    MouseScrolledEvent(float x, float y) : xOffset(x), yOffset(y) {}
};

/**
 * @brief 마우스 버튼 이벤트의 기반이 되는 구조체입니다.
 */
struct MouseButtonEvent : public Event {
    int button;

protected:
    MouseButtonEvent(int btn) : button(btn) {}
};

/**
 * @brief 마우스 버튼이 눌렸을 때 발생하는 이벤트입니다.
 */
struct MouseButtonPressedEvent : public MouseButtonEvent {
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}
};

/**
 * @brief 마우스 버튼에서 손을 뗐을 때 발생하는 이벤트입니다.
 */
struct MouseButtonReleasedEvent : public MouseButtonEvent {
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}
};
