#include "PlayerAnimationControlSystem.h"
#include <iostream>
#include <filesystem>
#include <cmath>
#include "engine/component/RenderComponent.h"
#include "engine/component/TransformComponent.h"

PlayerAnimationControlSystem::PlayerAnimationControlSystem(
    AnimationManager& animationManager,
    TextureManager& textureManager,
    RenderManager& renderManager
)
    : animationManager_(animationManager),
      textureManager_(textureManager),
      renderManager_(renderManager)
{}

void PlayerAnimationControlSystem::update(EntityManager& entityManager, float deltaTime) {
    for (EntityId entity : entityManager.getEntitiesWith<PlayerAnimationControllerComponent, AnimationComponent, VelocityComponent, TransformComponent>()) {
        PlayerAnimationControllerComponent* animController = entityManager.getComponent<PlayerAnimationControllerComponent>(entity);
        AnimationComponent* animation = entityManager.getComponent<AnimationComponent>(entity);
        VelocityComponent* velocity = entityManager.getComponent<VelocityComponent>(entity);

        // 이동 상태에 따른 애니메이션 전환 로직
        if (std::abs(velocity->vx) > 0.1f || std::abs(velocity->vy) > 0.1f) {
            // 움직이고 있다면 걷기 애니메이션 재생
            if (animation->animation_ != animController->walkAnimationData_) {
                playWalkAnimation(entityManager, entity, animController->walkAnimationData_);
            }
        } else {
            // 멈춰 있다면 첫 프레임으로 고정 (정지 애니메이션)
            if (animation->animation_ == animController->walkAnimationData_) {
                animation->stop(); // 애니메이션 정지
                animation->currentFrame_ = 0; // 첫 프레임으로
            }
        }
    }
}

void PlayerAnimationControlSystem::playWalkAnimation(EntityManager& entityManager, EntityId entityId, std::shared_ptr<Animation> walkAnimationData) {
    setCurrentAnimation(entityManager, entityId, walkAnimationData);
}

void PlayerAnimationControlSystem::playJumpAnimation(EntityManager& entityManager, EntityId entityId, std::shared_ptr<Animation> jumpAnimationData) {
    setCurrentAnimation(entityManager, entityId, jumpAnimationData);
}

bool PlayerAnimationControlSystem::isJumpAnimationActive(AnimationComponent* animation, std::shared_ptr<Animation> jumpAnimationData) const {
    return animation && animation->animation_ == jumpAnimationData;
}

void PlayerAnimationControlSystem::setCurrentAnimation(EntityManager& entityManager, EntityId entityId, std::shared_ptr<Animation> newAnimation) {
    if (!newAnimation) {
        std::cerr << "Error: Attempted to set a null animation for entity " << entityId << std::endl;
        if (entityManager.hasComponent<AnimationComponent>(entityId)) {
            entityManager.removeComponent<AnimationComponent>(entityId);
        }
        if (entityManager.hasComponent<RenderComponent>(entityId)) {
            entityManager.removeComponent<RenderComponent>(entityId);
        }
        return;
    }

    // 기존 AnimationComponent 제거
    if (entityManager.hasComponent<AnimationComponent>(entityId)) {
        entityManager.removeComponent<AnimationComponent>(entityId);
    }

    // 새로운 AnimationComponent 추가
    entityManager.addComponent<AnimationComponent>(entityId, newAnimation);

    // RenderComponent 업데이트
    if (entityManager.hasComponent<RenderComponent>(entityId)) {
        entityManager.removeComponent<RenderComponent>(entityId);
    }

    // 텍스처 로드 시도
    if (textureManager_.loadTexture(newAnimation->getTexturePath())) {
        Texture* newAnimTexture = textureManager_.getTexture(newAnimation->getTexturePath());
        if (newAnimTexture) {
            entityManager.addComponent<RenderComponent>(entityId, newAnimTexture, true);
        } else {
            std::cerr << "Error: Could not get texture after loading for new animation: " << newAnimation->getTexturePath() << ". RenderComponent not added." << std::endl;
        }
    } else {
        std::cerr << "Error: Could not load texture for new animation: " << newAnimation->getTexturePath() << ". RenderComponent not added." << std::endl;
    }

    // Fallback if new animation setup failed
    if (!entityManager.hasComponent<RenderComponent>(entityId)) {
        std::filesystem::path defaultTexturePath = std::filesystem::path(IMAGE_ASSET_ROOT_PATH) / "example_png.png";
        if (textureManager_.loadTexture(defaultTexturePath)) {
            Texture* defaultTexture = textureManager_.getTexture(defaultTexturePath);
            if (defaultTexture) {
                entityManager.addComponent<RenderComponent>(entityId, defaultTexture, false);
            } else {
                std::cerr << "Error: Could not get default texture for fallback: " << defaultTexturePath << ". RenderComponent not added." << std::endl;
            }
        } else {
            std::cerr << "Error: Could not load default texture for fallback: " << defaultTexturePath << ". RenderComponent not added." << std::endl;
        }
    }
}
