#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <format>

#include "Entity.h"
#include "GNEngine/component/TransformComponent.h"
#include "GNEngine/component/VelocityComponent.h"
#include "GNEngine/component/AccelerationComponent.h"
#include "GNEngine/component/RenderComponent.h"
#include "GNEngine/component/AnimationComponent.h"
#include "GNEngine/component/TextComponent.h"
#include "GNEngine/component/CameraComponent.h"

class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(EntityID entity) = 0;
    virtual bool hasComponent(EntityID entity) const = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public:
    void addComponent(EntityID entity, T&& component) {
        if (entityToIndexMap.count(entity)) {
            throw std::runtime_error("Component already added to entity.");
        }
        size_t newIndex = components.size();
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;
        components.push_back(std::move(component));
    }

    void removeComponent(EntityID entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("Component not found for entity.");
        }
        size_t indexOfRemoved = entityToIndexMap[entity];
        size_t indexOfLast = components.size() - 1;
        components[indexOfRemoved] = std::move(components[indexOfLast]);
        
        EntityID entityOfLast = indexToEntityMap[indexOfLast];
        entityToIndexMap[entityOfLast] = indexOfRemoved;
        indexToEntityMap[indexOfRemoved] = entityOfLast;

        components.pop_back();
        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLast);
    }

    T& getComponent(EntityID entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("Component not found for entity.");
        }
        return components[entityToIndexMap.at(entity)];
    }

    bool hasComponent(EntityID entity) const override {
        return entityToIndexMap.count(entity);
    }

    void entityDestroyed(EntityID entity) override {
        if (entityToIndexMap.count(entity)) {
            removeComponent(entity);
        }
    }

protected:
    std::vector<T> components;
    std::unordered_map<EntityID, size_t> entityToIndexMap;
    std::unordered_map<size_t, EntityID> indexToEntityMap;
};

class GNEngine_API SoAComponentArray : public IComponentArray {
public:
    void entityDestroyed(EntityID entity) override;

    bool hasComponent(EntityID entity) const override {
        return entityToIndexMap.count(entity);
    }

    const std::unordered_map<EntityID, size_t>& getEntityToIndexMap() const {
        return entityToIndexMap;
    }

protected:
    virtual void swapAndPop(size_t indexOfRemoved, size_t indexOfLast) = 0;

    static std::unordered_map<EntityID, size_t> entityToIndexMap;
    static std::unordered_map<size_t, EntityID> indexToEntityMap;
};


template<>
class ComponentArray<TransformComponent> : public SoAComponentArray {
public:
    void addComponent(EntityID entity, TransformComponent&& component) {
        size_t index;
        auto it = entityToIndexMap.find(entity);
        if (it == entityToIndexMap.end()) {
            index = indexToEntityMap.size();
            entityToIndexMap[entity] = index;
            indexToEntityMap[index] = entity;
        } else {
            index = it->second;
        }

        if (index >= positionX.size()) {
            positionX.resize(index + 1);
            positionY.resize(index + 1);
            scaleX.resize(index + 1);
            scaleY.resize(index + 1);
            rotatedAngle.resize(index + 1);
        }

        positionX[index] = component.positionX_;
        positionY[index] = component.positionY_;
        scaleX[index] = component.scaleX_;
        scaleY[index] = component.scaleY_;
        rotatedAngle[index] = component.rotatedAngle_;
    }

    void removeComponent(EntityID entity) { /* Stub */ }

    TransformComponent getComponent(EntityID entity) {
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

    std::vector<float> positionX;
    std::vector<float> positionY;
    std::vector<float> scaleX;
    std::vector<float> scaleY;
    std::vector<float> rotatedAngle;

protected:
    void swapAndPop(size_t indexOfRemoved, size_t indexOfLast) override {
        positionX[indexOfRemoved] = positionX[indexOfLast];
        positionY[indexOfRemoved] = positionY[indexOfLast];
        scaleX[indexOfRemoved] = scaleX[indexOfLast];
        scaleY[indexOfRemoved] = scaleY[indexOfLast];
        rotatedAngle[indexOfRemoved] = rotatedAngle[indexOfLast];

        positionX.pop_back();
        positionY.pop_back();
        scaleX.pop_back();
        scaleY.pop_back();
        rotatedAngle.pop_back();
    }
};

template<>
class ComponentArray<VelocityComponent> : public SoAComponentArray {
public:
    void addComponent(EntityID entity, VelocityComponent&& component) {
        size_t index;
        auto it = entityToIndexMap.find(entity);
        if (it == entityToIndexMap.end()) {
            index = indexToEntityMap.size();
            entityToIndexMap[entity] = index;
            indexToEntityMap[index] = entity;
        } else {
            index = it->second;
        }

        if (index >= vx.size()) {
            vx.resize(index + 1);
            vy.resize(index + 1);
        }
        vx[index] = component.vx;
        vy[index] = component.vy;
    }

    void removeComponent(EntityID entity) { /* Stub */ }

    VelocityComponent getComponent(EntityID entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("VelocityComponent not found for entity.");
        }
        size_t index = entityToIndexMap.at(entity);
        return VelocityComponent{vx[index], vy[index]};
    }

    std::vector<float> vx;
    std::vector<float> vy;

protected:
    void swapAndPop(size_t indexOfRemoved, size_t indexOfLast) override {
        vx[indexOfRemoved] = vx[indexOfLast];
        vy[indexOfRemoved] = vy[indexOfLast];
        vx.pop_back();
        vy.pop_back();
    }
};

template<>
class ComponentArray<AccelerationComponent> : public SoAComponentArray {
public:
    void addComponent(EntityID entity, AccelerationComponent&& component) {
        size_t index;
        auto it = entityToIndexMap.find(entity);
        if (it == entityToIndexMap.end()) {
            index = indexToEntityMap.size();
            entityToIndexMap[entity] = index;
            indexToEntityMap[index] = entity;
        } else {
            index = it->second;
        }

        if (index >= ax.size()) {
            ax.resize(index + 1);
            ay.resize(index + 1);
        }
        ax[index] = component.ax;
        ay[index] = component.ay;
    }

    void removeComponent(EntityID entity) { /* Stub */ }

    AccelerationComponent getComponent(EntityID entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("AccelerationComponent not found for entity.");
        }
        size_t index = entityToIndexMap.at(entity);
        return AccelerationComponent{ax[index], ay[index]};
    }

    std::vector<float> ax;
    std::vector<float> ay;

protected:
    void swapAndPop(size_t indexOfRemoved, size_t indexOfLast) override {
        ax[indexOfRemoved] = ax[indexOfLast];
        ay[indexOfRemoved] = ay[indexOfLast];
        ax.pop_back();
        ay.pop_back();
    }
};


template<>
class ComponentArray<RenderComponent> : public SoAComponentArray {
public:
    void addComponent(EntityID entity, RenderComponent&& component) {
        size_t index;
        auto it = entityToIndexMap.find(entity);
        if (it == entityToIndexMap.end()) {
            index = indexToEntityMap.size();
            entityToIndexMap[entity] = index;
            indexToEntityMap[index] = entity;
        } else {
            index = it->second;
        }

        if (index >= textures.size()) {
            textures.resize(index + 1);
            layers.resize(index + 1);
            hasAnimations.resize(index + 1);
            srcRectX.resize(index + 1);
            srcRectY.resize(index + 1);
            srcRectW.resize(index + 1);
            srcRectH.resize(index + 1);
            flipX.resize(index + 1);
            flipY.resize(index + 1);
        }

        textures[index] = component.getTexture();
        layers[index] = component.getLayer();
        hasAnimations[index] = component.hasAnimation();
        const auto& rect = component.getSrcRect();
        srcRectX[index] = rect.x;
        srcRectY[index] = rect.y;
        srcRectW[index] = rect.w;
        srcRectH[index] = rect.h;
        flipX[index] = component.getFlipX();
        flipY[index] = component.getFlipY();
    }

    void removeComponent(EntityID entity) { /* Stub */ }

    RenderComponent getComponent(EntityID entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("RenderComponent not found for entity.");
        }
        size_t i = entityToIndexMap.at(entity);
        return RenderComponent(textures[i], layers[i], hasAnimations[i], {srcRectX[i], srcRectY[i], srcRectW[i], srcRectH[i]}, flipX[i], flipY[i]);
    }

    std::vector<Texture*> textures;
    std::vector<RenderLayer> layers;
    std::vector<bool> hasAnimations;
    std::vector<int> srcRectX, srcRectY, srcRectW, srcRectH;
    std::vector<bool> flipX, flipY;

protected:
    void swapAndPop(size_t indexOfRemoved, size_t indexOfLast) override {
        textures[indexOfRemoved] = textures[indexOfLast];
        layers[indexOfRemoved] = layers[indexOfLast];
        hasAnimations[indexOfRemoved] = hasAnimations[indexOfLast];
        srcRectX[indexOfRemoved] = srcRectX[indexOfLast];
        srcRectY[indexOfRemoved] = srcRectY[indexOfLast];
        srcRectW[indexOfRemoved] = srcRectW[indexOfLast];
        srcRectH[indexOfRemoved] = srcRectH[indexOfLast];
        flipX[indexOfRemoved] = flipX[indexOfLast];
        flipY[indexOfRemoved] = flipY[indexOfLast];

        textures.pop_back();
        layers.pop_back();
        hasAnimations.pop_back();
        srcRectX.pop_back();
        srcRectY.pop_back();
        srcRectW.pop_back();
        srcRectH.pop_back();
        flipX.pop_back();
        flipY.pop_back();
    }
};

template<>
class ComponentArray<AnimationComponent> : public SoAComponentArray {
public:
    void addComponent(EntityID entity, AnimationComponent&& component) {
        size_t index;
        auto it = entityToIndexMap.find(entity);
        if (it == entityToIndexMap.end()) {
            index = indexToEntityMap.size();
            entityToIndexMap[entity] = index;
            indexToEntityMap[index] = entity;
        } else {
            index = it->second;
        }

        if (index >= animations.size()) {
            animations.resize(index + 1);
            currentFrames.resize(index + 1);
            frameTimers.resize(index + 1);
            arePlaying.resize(index + 1);
            areFinished.resize(index + 1);
        }

        animations[index] = component.animation_;
        currentFrames[index] = component.currentFrame_;
        frameTimers[index] = component.frameTimer_;
        arePlaying[index] = component.isPlaying_;
        areFinished[index] = component.isFinished_;
    }

    void removeComponent(EntityID entity) { /* Stub */ }

    AnimationComponent getComponent(EntityID entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("AnimationComponent not found for entity.");
        }
        size_t i = entityToIndexMap.at(entity);
        AnimationComponent comp(animations[i]);
        comp.currentFrame_ = currentFrames[i];
        comp.frameTimer_ = frameTimers[i];
        comp.isPlaying_ = arePlaying[i];
        comp.isFinished_ = areFinished[i];
        return comp;
    }

    std::vector<std::shared_ptr<Animation>> animations;
    std::vector<int> currentFrames;
    std::vector<float> frameTimers;
    std::vector<bool> arePlaying;
    std::vector<bool> areFinished;

protected:
    void swapAndPop(size_t indexOfRemoved, size_t indexOfLast) override {
        animations[indexOfRemoved] = std::move(animations[indexOfLast]);
        currentFrames[indexOfRemoved] = currentFrames[indexOfLast];
        frameTimers[indexOfRemoved] = frameTimers[indexOfLast];
        arePlaying[indexOfRemoved] = arePlaying[indexOfLast];
        areFinished[indexOfRemoved] = areFinished[indexOfLast];

        animations.pop_back();
        currentFrames.pop_back();
        frameTimers.pop_back();
        arePlaying.pop_back();
        areFinished.pop_back();
    }
};

template<>
class ComponentArray<TextComponent> : public SoAComponentArray {
public:
    void addComponent(EntityID entity, TextComponent&& component) {
        size_t index;
        auto it = entityToIndexMap.find(entity);
        if (it == entityToIndexMap.end()) {
            index = indexToEntityMap.size();
            entityToIndexMap[entity] = index;
            indexToEntityMap[index] = entity;
        } else {
            index = it->second;
        }

        if (index >= texts.size()) {
            texts.resize(index + 1);
            fontPaths.resize(index + 1);
            fontSizes.resize(index + 1);
            colorsR.resize(index + 1);
            colorsG.resize(index + 1);
            colorsB.resize(index + 1);
            colorsA.resize(index + 1);
            areDirty.resize(index + 1);
            textures.resize(index + 1);
            textureWidths.resize(index + 1);
            textureHeights.resize(index + 1);
        }

        texts[index] = std::move(component.text);
        fontPaths[index] = std::move(component.fontPath);
        fontSizes[index] = component.fontSize;
        colorsR[index] = component.color.r;
        colorsG[index] = component.color.g;
        colorsB[index] = component.color.b;
        colorsA[index] = component.color.a;
        areDirty[index] = component.isDirty;
        textures[index] = component.texture;
        textureWidths[index] = component.textureWidth;
        textureHeights[index] = component.textureHeight;
    }

    void removeComponent(EntityID entity) { /* Stub */ }

    TextComponent getComponent(EntityID entity) {
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

    std::vector<std::string> texts;
    std::vector<std::filesystem::path> fontPaths;
    std::vector<int> fontSizes;
    std::vector<Uint8> colorsR, colorsG, colorsB, colorsA;
    std::vector<bool> areDirty;
    std::vector<SDL_Texture*> textures;
    std::vector<float> textureWidths;
    std::vector<float> textureHeights;

protected:
    void swapAndPop(size_t i, size_t last_i) override {
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
        textures[i] = textures[last_i];
        textureWidths[i] = textureWidths[last_i];
        textureHeights[i] = textureHeights[last_i];

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
    }
};

template<>
class ComponentArray<CameraComponent> : public SoAComponentArray {
public:
    void addComponent(EntityID entity, CameraComponent&& component) {
        size_t index;
        auto it = entityToIndexMap.find(entity);
        if (it == entityToIndexMap.end()) {
            index = indexToEntityMap.size();
            entityToIndexMap[entity] = index;
            indexToEntityMap[index] = entity;
        } else {
            index = it->second;
        }

        if (index >= x.size()) {
            x.resize(index + 1);
            y.resize(index + 1);
            zoom.resize(index + 1);
            targetEntityIds.resize(index + 1);
        }

        x[index] = component.x;
        y[index] = component.y;
        zoom[index] = component.zoom;
        targetEntityIds[index] = component.targetEntityId;
    }

    void removeComponent(EntityID entity) { /* Stub */ }

    CameraComponent getComponent(EntityID entity) {
        if (!entityToIndexMap.count(entity)) {
            throw std::runtime_error("CameraComponent not found for entity.");
        }
        size_t i = entityToIndexMap.at(entity);
        return CameraComponent(x[i], y[i], zoom[i], targetEntityIds[i]);
    }

    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> zoom;
    std::vector<EntityID> targetEntityIds;

protected:
    void swapAndPop(size_t indexOfRemoved, size_t indexOfLast) override {
        x[indexOfRemoved] = x[indexOfLast];
        y[indexOfRemoved] = y[indexOfLast];
        zoom[indexOfRemoved] = zoom[indexOfLast];
        targetEntityIds[indexOfRemoved] = targetEntityIds[indexOfLast];

        x.pop_back();
        y.pop_back();
        zoom.pop_back();
        targetEntityIds.pop_back();
    }
};
