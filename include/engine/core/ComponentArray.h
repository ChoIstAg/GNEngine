#pragma once

#include "Entity.h"
#include "engine/component/TransformComponent.h"
#include "engine/component/VelocityComponent.h"
#include "engine/component/AccelerationComponent.h"
#include "engine/component/RenderComponent.h"
#include "engine/component/AnimationComponent.h"
#include "engine/component/TextComponent.h"
#include "engine/component/CameraComponent.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>

/*
 * @brief 모든 컴포넌트 배열의 기본 인터페이스임.
*/
class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(EntityId entity) = 0;
    virtual bool hasComponent(EntityId entity) const = 0;
};

/*
 * @brief 특정 타입 T의 컴포넌트를 저장하는 기본 배열 (AoS 방식)
*/
template<typename T>
class ComponentArray : public IComponentArray {
public:
    void addComponent(EntityId entity, T&& component) {
        if (entityToIndexMap.count(entity)) {
            throw std::runtime_error("Component already added to entity.");
        }
        size_t newIndex = components.size();
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;
        components.push_back(std::move(component));
    }

    void removeComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("Component not found for entity.");
        }
        size_t indexOfRemoved = entityToIndexMap[entity];
        size_t indexOfLast = components.size() - 1;
        components[indexOfRemoved] = std::move(components[indexOfLast]);
        
        EntityId entityOfLast = indexToEntityMap[indexOfLast];
        entityToIndexMap[entityOfLast] = indexOfRemoved;
        indexToEntityMap[indexOfRemoved] = entityOfLast;

        components.pop_back();
        entityToIndexMap.erase(entity);
        entityToIndexMap.erase(indexOfLast);
    }

    T& getComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("Component not found for entity.");
        }
        return components[entityToIndexMap.at(entity)];
    }

    bool hasComponent(EntityId entity) const override {
        return entityToIndexMap.count(entity);
    }

    void entityDestroyed(EntityId entity) override {
        if (entityToIndexMap.count(entity)) {
            removeComponent(entity);
        }
    }

protected:
    std::vector<T> components;
    std::unordered_map<EntityId, size_t> entityToIndexMap;
    std::unordered_map<size_t, EntityId> indexToEntityMap;
};

/*
 * @brief TransformComponent를 위한 템플릿 특수화 (SoA 방식)
*/
template<>
class ComponentArray<TransformComponent> : public IComponentArray {
public:
    void addComponent(EntityId entity, TransformComponent&& component) {
        if (entityToIndexMap.count(entity)) {
            throw std::runtime_error("TransformComponent already added to entity.");
        }
        size_t newIndex = positionX.size();
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;

        positionX.push_back(component.positionX_);
        positionY.push_back(component.positionY_);
        scaleX.push_back(component.scaleX_);
        scaleY.push_back(component.scaleY_);
        rotatedAngle.push_back(component.rotatedAngle_);
    }

    void removeComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("TransformComponent not found for entity.");
        }
        size_t indexOfRemoved = entityToIndexMap[entity];
        size_t indexOfLast = positionX.size() - 1;

        positionX[indexOfRemoved] = positionX[indexOfLast];
        positionY[indexOfRemoved] = positionY[indexOfLast];
        scaleX[indexOfRemoved] = scaleX[indexOfLast];
        scaleY[indexOfRemoved] = scaleY[indexOfLast];
        rotatedAngle[indexOfRemoved] = rotatedAngle[indexOfLast];

        EntityId entityOfLast = indexToEntityMap[indexOfLast];
        entityToIndexMap[entityOfLast] = indexOfRemoved;
        indexToEntityMap[indexOfRemoved] = entityOfLast;

        positionX.pop_back();
        positionY.pop_back();
        scaleX.pop_back();
        scaleY.pop_back();
        rotatedAngle.pop_back();

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLast);
    }

    TransformComponent getComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("TransformComponent not found for entity.");
        }
        size_t index = entityToIndexMap.at(entity);
        return TransformComponent{
            positionX[index],
            positionY[index],
            scaleX[index],
            scaleY[index],
            rotatedAngle[index]
        };
    }

    bool hasComponent(EntityId entity) const override {
        return entityToIndexMap.count(entity);
    }

    void entityDestroyed(EntityId entity) override {
        if (entityToIndexMap.count(entity)) {
            removeComponent(entity);
        }
    }

    std::vector<float> positionX;
    std::vector<float> positionY;
    std::vector<float> scaleX;
    std::vector<float> scaleY;
    std::vector<float> rotatedAngle;

    const std::unordered_map<EntityId, size_t>& getEntityToIndexMap() const {
        return entityToIndexMap;
    }

public:
    std::unordered_map<EntityId, size_t> entityToIndexMap;
    std::unordered_map<size_t, EntityId> indexToEntityMap;
};

/*
 * @brief VelocityComponent를 위한 템플릿 특수화 (SoA 방식)
*/
template<>
class ComponentArray<VelocityComponent> : public IComponentArray {
public:
    void addComponent(EntityId entity, VelocityComponent&& component) {
        if (entityToIndexMap.count(entity)) {
            throw std::runtime_error("VelocityComponent already added to entity.");
        }
        size_t newIndex = vx.size();
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;
        vx.push_back(component.vx);
        vy.push_back(component.vy);
    }

    void removeComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("VelocityComponent not found for entity.");
        }
        size_t indexOfRemoved = entityToIndexMap[entity];
        size_t indexOfLast = vx.size() - 1;

        vx[indexOfRemoved] = vx[indexOfLast];
        vy[indexOfRemoved] = vy[indexOfLast];

        EntityId entityOfLast = indexToEntityMap[indexOfLast];
        entityToIndexMap[entityOfLast] = indexOfRemoved;
        indexToEntityMap[indexOfRemoved] = entityOfLast;

        vx.pop_back();
        vy.pop_back();

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLast);
    }

    VelocityComponent getComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("VelocityComponent not found for entity.");
        }
        size_t index = entityToIndexMap.at(entity);
        return VelocityComponent{vx[index], vy[index]};
    }

    bool hasComponent(EntityId entity) const override {
        return entityToIndexMap.count(entity);
    }

    void entityDestroyed(EntityId entity) override {
        if (entityToIndexMap.count(entity)) {
            removeComponent(entity);
        }
    }

    std::vector<float> vx;
    std::vector<float> vy;

    const std::unordered_map<EntityId, size_t>& getEntityToIndexMap() const {
        return entityToIndexMap;
    }

public:
    std::unordered_map<EntityId, size_t> entityToIndexMap;
    std::unordered_map<size_t, EntityId> indexToEntityMap;
};

/*
 * @brief AccelerationComponent를 위한 템플릿 특수화 (SoA 방식)
*/
template<>
class ComponentArray<AccelerationComponent> : public IComponentArray {
public:
    void addComponent(EntityId entity, AccelerationComponent&& component) {
        if (entityToIndexMap.count(entity)) {
            throw std::runtime_error("AccelerationComponent already added to entity.");
        }
        size_t newIndex = ax.size();
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;
        ax.push_back(component.ax);
        ay.push_back(component.ay);
    }

    void removeComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("AccelerationComponent not found for entity.");
        }
        size_t indexOfRemoved = entityToIndexMap[entity];
        size_t indexOfLast = ax.size() - 1;

        ax[indexOfRemoved] = ax[indexOfLast];
        ay[indexOfRemoved] = ay[indexOfLast];

        EntityId entityOfLast = indexToEntityMap[indexOfLast];
        entityToIndexMap[entityOfLast] = indexOfRemoved;
        indexToEntityMap[indexOfRemoved] = entityOfLast;

        ax.pop_back();
        ay.pop_back();

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLast);
    }

    AccelerationComponent getComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("AccelerationComponent not found for entity.");
        }
        size_t index = entityToIndexMap.at(entity);
        return AccelerationComponent{ax[index], ay[index]};
    }

    bool hasComponent(EntityId entity) const override {
        return entityToIndexMap.count(entity);
    }

    void entityDestroyed(EntityId entity) override {
        if (entityToIndexMap.count(entity)) {
            removeComponent(entity);
        }
    }

    std::vector<float> ax;
    std::vector<float> ay;

    const std::unordered_map<EntityId, size_t>& getEntityToIndexMap() const {
        return entityToIndexMap;
    }

public:
    std::unordered_map<EntityId, size_t> entityToIndexMap;
    std::unordered_map<size_t, EntityId> indexToEntityMap;
};


/*
 * @brief RenderComponent를 위한 템플릿 특수화 (SoA 방식)
*/
template<>
class ComponentArray<RenderComponent> : public IComponentArray {
public:
    void addComponent(EntityId entity, RenderComponent&& component) {
        if (entityToIndexMap.count(entity)) {
            throw std::runtime_error("RenderComponent already added to entity.");
        }
        size_t newIndex = textures.size();
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;

        textures.push_back(component.getTexture());
        hasAnimations.push_back(component.hasAnimation());
        const auto& rect = component.getSrcRect();
        srcRectX.push_back(rect.x);
        srcRectY.push_back(rect.y);
        srcRectW.push_back(rect.w);
        srcRectH.push_back(rect.h);
        flipX.push_back(component.getFlipX());
        flipY.push_back(component.getFlipY());
    }

    void removeComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("RenderComponent not found for entity.");
        }
        size_t indexOfRemoved = entityToIndexMap.at(entity);
        size_t indexOfLast = textures.size() - 1;

        // 모든 SoA 배열에 대해 마지막 요소를 삭제 위치로 이동
        textures[indexOfRemoved] = textures[indexOfLast];
        hasAnimations[indexOfRemoved] = hasAnimations[indexOfLast];
        srcRectX[indexOfRemoved] = srcRectX[indexOfLast];
        srcRectY[indexOfRemoved] = srcRectY[indexOfLast];
        srcRectW[indexOfRemoved] = srcRectW[indexOfLast];
        srcRectH[indexOfRemoved] = srcRectH[indexOfLast];
        flipX[indexOfRemoved] = flipX[indexOfLast];
        flipY[indexOfRemoved] = flipY[indexOfLast];

        // 매핑 정보 업데이트
        EntityId entityOfLast = indexToEntityMap.at(indexOfLast);
        entityToIndexMap[entityOfLast] = indexOfRemoved;
        indexToEntityMap[indexOfRemoved] = entityOfLast;

        // 모든 SoA 배열에서 마지막 요소 제거
        textures.pop_back();
        hasAnimations.pop_back();
        srcRectX.pop_back();
        srcRectY.pop_back();
        srcRectW.pop_back();
        srcRectH.pop_back();
        flipX.pop_back();
        flipY.pop_back();

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLast);
    }

    RenderComponent getComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("RenderComponent not found for entity.");
        }
        size_t i = entityToIndexMap.at(entity);
        return RenderComponent(textures[i], hasAnimations[i], {srcRectX[i], srcRectY[i], srcRectW[i], srcRectH[i]}, flipX[i], flipY[i]);
    }

    bool hasComponent(EntityId entity) const override {
        return entityToIndexMap.count(entity);
    }

    void entityDestroyed(EntityId entity) override {
        if (entityToIndexMap.count(entity)) {
            removeComponent(entity);
        }
    }

    std::vector<Texture*> textures;
    std::vector<bool> hasAnimations;
    std::vector<int> srcRectX, srcRectY, srcRectW, srcRectH;
    std::vector<bool> flipX, flipY;

    const std::unordered_map<EntityId, size_t>& getEntityToIndexMap() const {
        return entityToIndexMap;
    }

public:
    std::unordered_map<EntityId, size_t> entityToIndexMap;
    std::unordered_map<size_t, EntityId> indexToEntityMap;
};

/*
 * @brief AnimationComponent를 위한 템플릿 특수화 (SoA 방식)
*/
template<>
class ComponentArray<AnimationComponent> : public IComponentArray {
public:
    void addComponent(EntityId entity, AnimationComponent&& component) {
        if (entityToIndexMap.count(entity)) {
            throw std::runtime_error("AnimationComponent already added to entity.");
        }
        size_t newIndex = animations.size();
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;

        animations.push_back(component.animation_);
        currentFrames.push_back(component.currentFrame_);
        frameTimers.push_back(component.frameTimer_);
        arePlaying.push_back(component.isPlaying_);
        areFinished.push_back(component.isFinished_);
    }

    void removeComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("AnimationComponent not found for entity.");
        }
        size_t indexOfRemoved = entityToIndexMap.at(entity);
        size_t indexOfLast = animations.size() - 1;

        animations[indexOfRemoved] = std::move(animations[indexOfLast]);
        currentFrames[indexOfRemoved] = currentFrames[indexOfLast];
        frameTimers[indexOfRemoved] = frameTimers[indexOfLast];
        arePlaying[indexOfRemoved] = arePlaying[indexOfLast];
        areFinished[indexOfRemoved] = areFinished[indexOfLast];

        EntityId entityOfLast = indexToEntityMap.at(indexOfLast);
        entityToIndexMap[entityOfLast] = indexOfRemoved;
        indexToEntityMap[indexOfRemoved] = entityOfLast;

        animations.pop_back();
        currentFrames.pop_back();
        frameTimers.pop_back();
        arePlaying.pop_back();
        areFinished.pop_back();

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLast);
    }

    AnimationComponent getComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("AnimationComponent not found for entity.");
        }
        size_t i = entityToIndexMap.at(entity);
        AnimationComponent comp(animations[i]); // 생성자를 통해 기본값 설정
        comp.currentFrame_ = currentFrames[i];
        comp.frameTimer_ = frameTimers[i];
        comp.isPlaying_ = arePlaying[i];
        comp.isFinished_ = areFinished[i];
        return comp;
    }

    bool hasComponent(EntityId entity) const override {
        return entityToIndexMap.count(entity);
    }

    void entityDestroyed(EntityId entity) override {
        if (entityToIndexMap.count(entity)) {
            removeComponent(entity);
        }
    }

    std::vector<std::shared_ptr<Animation>> animations;
    std::vector<int> currentFrames;
    std::vector<float> frameTimers;
    std::vector<bool> arePlaying;
    std::vector<bool> areFinished;

    const std::unordered_map<EntityId, size_t>& getEntityToIndexMap() const {
        return entityToIndexMap;
    }

public:
    std::unordered_map<EntityId, size_t> entityToIndexMap;
    std::unordered_map<size_t, EntityId> indexToEntityMap;
};

/*
 * @brief TextComponent를 위한 템플릿 특수화 (SoA 방식)
*/
template<>
class ComponentArray<TextComponent> : public IComponentArray {
public:
    void addComponent(EntityId entity, TextComponent&& component) {
        if (entityToIndexMap.count(entity)) {
            throw std::runtime_error("TextComponent already added to entity.");
        }
        size_t newIndex = texts.size();
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;

        texts.push_back(std::move(component.text));
        fontPaths.push_back(std::move(component.fontPath));
        fontSizes.push_back(component.fontSize);
        colorsR.push_back(component.color.r);
        colorsG.push_back(component.color.g);
        colorsB.push_back(component.color.b);
        colorsA.push_back(component.color.a);
        areDirty.push_back(component.isDirty);
        textures.push_back(component.texture);
        textureWidths.push_back(component.textureWidth);
        textureHeights.push_back(component.textureHeight);
    }

    void removeComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("TextComponent not found for entity.");
        }
        size_t i = entityToIndexMap.at(entity);
        size_t last_i = texts.size() - 1;

        // 마지막 텍스처가 있다면 먼저 파괴
        if (textures[i] != nullptr) {
            SDL_DestroyTexture(textures[i]);
        }

        texts[i] = std::move(texts[last_i]);
        fontPaths[i] = std::move(fontPaths[last_i]);
        fontSizes[i] = fontSizes[last_i];
        colorsR[i] = colorsR[last_i];
        colorsG[i] = colorsG[last_i];
        colorsB[i] = colorsB[last_i];
        colorsA[i] = colorsA[last_i];
        areDirty[i] = areDirty[last_i];
        textures[i] = textures[last_i]; // 마지막 텍스처 포인터 이동
        textureWidths[i] = textureWidths[last_i];
        textureHeights[i] = textureHeights[last_i];

        EntityId lastEntity = indexToEntityMap.at(last_i);
        entityToIndexMap[lastEntity] = i;
        indexToEntityMap[i] = lastEntity;

        texts.pop_back();
        fontPaths.pop_back();
        fontSizes.pop_back();
        colorsR.pop_back();
        colorsG.pop_back();
        colorsB.pop_back();
        colorsA.pop_back();
        areDirty.pop_back();
        textures.pop_back();
        textureWidths.pop_back();
        textureHeights.pop_back();

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(last_i);
    }

    TextComponent getComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("TextComponent not found for entity.");
        }
        size_t i = entityToIndexMap.at(entity);
        TextComponent comp(texts[i], fontPaths[i], fontSizes[i], {colorsR[i], colorsG[i], colorsB[i], colorsA[i]});
        comp.isDirty = areDirty[i];
        comp.texture = textures[i];
        comp.textureWidth = textureWidths[i];
        comp.textureHeight = textureHeights[i];
        return comp;
    }

    bool hasComponent(EntityId entity) const override {
        return entityToIndexMap.count(entity);
    }

    void entityDestroyed(EntityId entity) override {
        if (entityToIndexMap.count(entity)) {
            removeComponent(entity);
        }
    }

    // SoA 데이터
    std::vector<std::string> texts;
    std::vector<std::filesystem::path> fontPaths;
    std::vector<int> fontSizes;
    std::vector<Uint8> colorsR, colorsG, colorsB, colorsA;
    std::vector<bool> areDirty;
    std::vector<SDL_Texture*> textures;
    std::vector<float> textureWidths;
    std::vector<float> textureHeights;

    const std::unordered_map<EntityId, size_t>& getEntityToIndexMap() const {
        return entityToIndexMap;
    }

public:
    std::unordered_map<EntityId, size_t> entityToIndexMap;
    std::unordered_map<size_t, EntityId> indexToEntityMap;
};

/*
 * @brief CameraComponent를 위한 템플릿 특수화 (SoA 방식)
*/
template<>
class ComponentArray<CameraComponent> : public IComponentArray {
public:
    void addComponent(EntityId entity, CameraComponent&& component) {
        if (entityToIndexMap.count(entity)) {
            throw std::runtime_error("CameraComponent already added to entity.");
        }
        size_t newIndex = x.size();
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;

        x.push_back(component.x);
        y.push_back(component.y);
        zoom.push_back(component.zoom);
        targetEntityIds.push_back(component.targetEntityId);
    }

    void removeComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("CameraComponent not found for entity.");
        }
        size_t indexOfRemoved = entityToIndexMap.at(entity);
        size_t indexOfLast = x.size() - 1;

        x[indexOfRemoved] = x[indexOfLast];
        y[indexOfRemoved] = y[indexOfLast];
        zoom[indexOfRemoved] = zoom[indexOfLast];
        targetEntityIds[indexOfRemoved] = targetEntityIds[indexOfLast];

        EntityId entityOfLast = indexToEntityMap.at(indexOfLast);
        entityToIndexMap[entityOfLast] = indexOfRemoved;
        indexToEntityMap[indexOfRemoved] = entityOfLast;

        x.pop_back();
        y.pop_back();
        zoom.pop_back();
        targetEntityIds.pop_back();

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLast);
    }

    CameraComponent getComponent(EntityId entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("CameraComponent not found for entity.");
        }
        size_t i = entityToIndexMap.at(entity);
        return CameraComponent(x[i], y[i], zoom[i], targetEntityIds[i]);
    }

    bool hasComponent(EntityId entity) const override {
        return entityToIndexMap.count(entity);
    }

    void entityDestroyed(EntityId entity) override {
        if (entityToIndexMap.count(entity)) {
            removeComponent(entity);
        }
    }

    // SoA 데이터
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> zoom;
    std::vector<EntityId> targetEntityIds;

    const std::unordered_map<EntityId, size_t>& getEntityToIndexMap() const {
        return entityToIndexMap;
    }

public:
    std::unordered_map<EntityId, size_t> entityToIndexMap;
    std::unordered_map<size_t, EntityId> indexToEntityMap;
};