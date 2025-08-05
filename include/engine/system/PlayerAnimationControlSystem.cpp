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
            if (animation->getAnimation() != animController->walkAnimationData_) {
                setCurrentAnimation(entityManager, entity, animController->walkAnimationData_);
            }
            if (!animation->isPlaying()) {
                animation->play();
            }
        } else {
            // 멈춰 있다면 idle 애니메이션 재생
            if (animation->getAnimation() != animController->idleAnimationData_) {
                setCurrentAnimation(entityManager, entity, animController->idleAnimationData_);
            }
            if (!animation->isPlaying()) {
                animation->play();
            }
        }

        // 방향에 따른 좌우 반전
        RenderComponent* render = entityManager.getComponent<RenderComponent>(entity);
        if (render) {
            if (velocity->vx < -0.1f) { // 왼쪽으로 이동 (기본 방향이 왼쪽이므로 반전 없음)
                render->setFlipX(false);
            } else if (velocity->vx > 0.1f) { // 오른쪽으로 이동 (오른쪽을 바라보도록 반전)
                render->setFlipX(true);
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

    // AnimationComponent 업데이트 (기존 컴포넌트 재활용 또는 새로 추가)
    AnimationComponent* existingAnimComp = entityManager.getComponent<AnimationComponent>(entityId);
    if (existingAnimComp) {
        existingAnimComp->animation_ = newAnimation;
        existingAnimComp->currentFrame_ = 0;
        existingAnimComp->frameTimer_ = 0.0f;
        existingAnimComp->play(); // 애니메이션 재생 시작
    } else {
        entityManager.addComponent<AnimationComponent>(entityId, newAnimation);
    }

    // RenderComponent 업데이트 (기존 컴포넌트 재활용 또는 새로 추가)
    RenderComponent* render = entityManager.getComponent<RenderComponent>(entityId);
    if (render) {
        // 텍스처 로드 시도
        if (textureManager_.loadTexture(newAnimation->getTexturePath())) {
            Texture* newAnimTexture = textureManager_.getTexture(newAnimation->getTexturePath());
            if (newAnimTexture) {
                render->setTexture(newAnimTexture);
                render->setSrcRect(newAnimation->getFrame(0)); // 첫 프레임으로 srcRect 설정
                render->setHasAnimation(true);
            } else {
                std::cerr << "Error: Could not get texture after loading for new animation: " << newAnimation->getTexturePath() << ". RenderComponent not updated." << std::endl;
            }
        } else {
            std::cerr << "Error: Could not load texture for new animation: " << newAnimation->getTexturePath() << ". RenderComponent not updated." << std::endl;
        }
    } else {
        // RenderComponent가 없으면 새로 추가
        if (textureManager_.loadTexture(newAnimation->getTexturePath())) {
            Texture* newAnimTexture = textureManager_.getTexture(newAnimation->getTexturePath());
            if (newAnimTexture) {
                entityManager.addComponent<RenderComponent>(entityId, newAnimTexture, true, newAnimation->getFrame(0));
            } else {
                std::cerr << "Error: Could not get texture after loading for new animation: " << newAnimation->getTexturePath() << ". RenderComponent not added." << std::endl;
            }
        } else {
            std::cerr << "Error: Could not load texture for new animation: " << newAnimation->getTexturePath() << ". RenderComponent not added." << std::endl;
        }
    }

    // Fallback if new animation setup failed (RenderComponent still not present)
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
