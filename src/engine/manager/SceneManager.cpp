#include "engine/manager/SceneManager.h"

#include <iostream>

#include "engine/core/Scene.h"

// SceneManager 생성자 구현
SceneManager::SceneManager() = default;

/*
 * addScene 함수는 새로운 씬을 씬 맵에 추가함.
 * @param name 씬의 고유 이름
 * @param scene 추가할 씬 객체의 unique_ptr
*/
// void SceneManager::addScene(const std::string& name, std::unique_ptr<Scene> scene) {
//     scenes_[name] = std::move(scene);
// } 템플릿으로 전환 시도중. 템플릿 코드는 SceneManager.h에..

/*
 * changeScene 함수는 현재 활성화된 씬을 변경함.
 * 이전 씬이 있다면 onExit을 호출하여 정리하고, 새로운 씬의 onEnter를 호출하여 초기화함.
 * @param name 변경할 씬의 이름
*/
void SceneManager::changeScene(const std::string& name) {
    // 현재 씬이 있다면 종료 처리
    if (currentScene_) {
        currentScene_->onExit();
    }

    // 새로운 씬을 찾아서 설정
    auto it = scenes_.find(name);
    if (it != scenes_.end()) {
        currentScene_ = it->second.get();
        // 새로운 씬 진입
        currentScene_->onEnter();
    } else {
        // 씬을 찾을 수 없는 경우 오류 처리 또는 로그
        std::cerr << "Error: Scene '" << name << "' not found." << std::endl;
        currentScene_ = nullptr;
    }
}

/*
 * handleEvent 함수는 현재 활성화된 씬의 이벤트를 처리합니다.
 * @param event 처리할 SDL 이벤트
*/
void SceneManager::handleEvent(const Event& event) {
    if (currentScene_) {
        currentScene_->handleEvent(event);
    }
}

/*
 * update 함수는 현재 활성화된 씬의 논리를 업데이트합니다.
 * @param deltaTime 이전 프레임으로부터 경과된 시간 (초)
*/
void SceneManager::update(float deltaTime) {
    if (currentScene_) {
        currentScene_->update(deltaTime);
    }
}

/*
 * render 함수는 현재 활성화된 씬을 렌더링합니다.
 * @param renderer 렌더링에 사용할 SDL_Renderer 포인터
*/
void SceneManager::render(SDL_Renderer* renderer) {
    if (currentScene_) {
        currentScene_->render(renderer);
    }
}