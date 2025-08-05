#include "PlayerPrefab.h"
#include "engine/manager/EntityManager.h"
#include "engine/manager/EventManager.h"
#include "engine/manager/TextureManager.h"
#include "engine/manager/RenderManager.h"
#include "engine/manager/SoundManager.h"
#include "engine/manager/AnimationManager.h"

#include "engine/component/TransformComponent.h"
#include "engine/component/VelocityComponent.h"
#include "engine/component/AccelerationComponent.h"
#include "engine/component/RenderComponent.h"
#include "engine/component/SoundComponent.h"
#include "engine/component/PlayerAnimationControllerComponent.h"
#include "engine/component/PlayerMovementComponent.h"
#include "engine/component/InputControlComponent.h" // InputControlComponent.h 추가
#include <iostream>
#include <filesystem>

/*
* @brief 플레이어 엔티티를 생성하고 필요한 모든 컴포넌트를 추가함.
* @param entityManager - 엔티티와 컴포넌트를 관리하는 EntityManager
* @param eventManager - 이벤트 시스템을 관리하는 EventManager
* @param textureManager - 텍스처 리소스를 관리하는 TextureManager
* @param renderManager - 렌더링을 총괄하는 RenderManager
* @param soundManager - 사운드 리소스를 관리하는 SoundManager
* @param animationManager - 애니메이션 리소스를 관리하는 AnimationManager
* @return 생성된 플레이어의 EntityId
*/
EntityId PlayerPrefab::create(EntityManager& entityManager, EventManager& eventManager, TextureManager& textureManager, RenderManager& renderManager, SoundManager& soundManager, AnimationManager& animationManager) {
    EntityId entityId = entityManager.createEntity();

    // 엔티티 생성 및 컴포넌트 추가
    entityManager.addComponent<TransformComponent>(entityId, 0.0f, 0.0f);
    entityManager.addComponent<VelocityComponent>(entityId, 0.0f, 0.0f);
    entityManager.addComponent<AccelerationComponent>(entityId, 0.0f, 0.0f);

    // PlayerAnimationControllerComponent 추가
    // 애니메이션 데이터 로드
    // T.C.json 파일 하나로 모든 애니메이션 로드
    std::filesystem::path tcAnimationJsonPath = std::filesystem::path(ANIMATION_SHEET_ASSET_ROOT_PATH) / "T.C/" / "T.C.json"; // T.C.json으로 경로 변경
    if (!animationManager.loadAnimation(tcAnimationJsonPath)) {
        std::cerr << "Error: Failed to load animation JSON: " << tcAnimationJsonPath << std::endl;
    }
    std::shared_ptr<Animation> tcWalkAnimationData = animationManager.getAnimation("walk");
    std::shared_ptr<Animation> tcJumpAnimationData = animationManager.getAnimation("jump");

    // RenderComponent 추가
    // 애니메이션 데이터에서 텍스처 경로를 가져와 TextureManager를 통해 로드
    if (tcWalkAnimationData) {
        std::filesystem::path texturePath = tcWalkAnimationData->getTexturePath();
        if (!textureManager.loadTexture(texturePath)) {
            std::cerr << "Error: Failed to load texture for animation: " << texturePath << std::endl;
        }
        Texture* animationTexture = textureManager.getTexture(texturePath);
        if (animationTexture) {
            entityManager.addComponent<RenderComponent>(entityId, animationTexture, true, tcWalkAnimationData->getFrame(0), SDL_FLIP_NONE);
        } else {
            std::cerr << "Error: Animation texture is null for: " << texturePath << std::endl;
        }
    } else {
        std::cerr << "Error: Walk animation data is null. Cannot add RenderComponent." << std::endl;
    }

    std::shared_ptr<Animation> tcIdleAnimationData = animationManager.getAnimation("idle");

    entityManager.addComponent<PlayerAnimationControllerComponent>(entityId, tcWalkAnimationData, tcJumpAnimationData, tcIdleAnimationData);

    // AnimationComponent 추가: PlayerAnimationControllerComponent에서 사용할 기본 애니메이션으로 초기화
    AnimationComponent& animComp = entityManager.addComponent<AnimationComponent>(entityId, tcWalkAnimationData);

    // SoundComponent 추가
    std::filesystem::path soundPath = std::filesystem::path(SOUND_EFFECT_ASSET_ROOT_PATH) / "hit01.flac";
    std::shared_ptr<Sound> hitSound = soundManager.getSound(soundPath);
    if (hitSound) {
        entityManager.addComponent<SoundComponent>(entityId, soundManager, hitSound);
    }

    // PlayerMovementComponent 제거 (PlayerInputComponent와 MovementSystem이 대체)
    // entityManager.removeComponent<PlayerMovementComponent>(entityId); // 이전에 추가된 경우 제거

    // InputControlComponent 추가
    entityManager.addComponent<InputControlComponent>(entityId);

    return entityId;
}