#include "TestObject.h"
#include <iostream>
#include <filesystem>

// TestObject는 더 이상 Object를 상속받지 않으므로, 컴포넌트 추가 로직을 직접 구현해야 합니다.
// 이 단계에서는 EntityManager를 통해 컴포넌트를 추가합니다.

TestObject::TestObject(EntityManager& entityManager, EventManager& eventManager, TextureManager& textureManager, RenderManager& renderManager, SoundManager& soundManager, AnimationManager& animationManager)
    : entityId_(entityManager.createEntity())
{
    // TransformComponent 추가
    entityManager.addComponent<TransformComponent>(entityId_, 100.0f, 100.0f);
    // VelocityComponent 추가
    entityManager.addComponent<VelocityComponent>(entityId_, 0.0f, 0.0f);
    // AccelerationComponent 추가
    entityManager.addComponent<AccelerationComponent>(entityId_, 0.0f, 0.0f);

    // PlayerAnimationControllerComponent 추가
    // 애니메이션 데이터 로드
    std::filesystem::path walkAnimationJsonPath = std::filesystem::path(ANIMATION_SHEET_ASSET_ROOT_PATH) / "walk" / "walk.json";
    if (!animationManager.loadAnimation(walkAnimationJsonPath)) {
        std::cerr << "Error: Failed to load animation JSON: " << walkAnimationJsonPath << std::endl;
    }
    std::shared_ptr<Animation> walkAnimationData = animationManager.getAnimation("walk");

    std::filesystem::path jumpAnimationJsonPath = std::filesystem::path(ANIMATION_SHEET_ASSET_ROOT_PATH) / "jump" / "jump.json";
    if (!animationManager.loadAnimation(jumpAnimationJsonPath)) {
        std::cerr << "Error: Failed to load animation JSON: " << jumpAnimationJsonPath << std::endl;
    }
    std::shared_ptr<Animation> jumpAnimationData = animationManager.getAnimation("jump");

    entityManager.addComponent<PlayerAnimationControllerComponent>(entityId_, walkAnimationData, jumpAnimationData);

    // SoundComponent 추가
    std::filesystem::path soundPath = std::filesystem::path(SOUND_EFFECT_ASSET_ROOT_PATH) / "hit01.flac";
    std::shared_ptr<Sound> hitSound = soundManager.getSound(soundPath);

    if (hitSound) {
        entityManager.addComponent<SoundComponent>(entityId_, soundManager, hitSound);
    }

    // PlayerMovementComponent 추가
    entityManager.addComponent<PlayerMovementComponent>(entityId_);
}


