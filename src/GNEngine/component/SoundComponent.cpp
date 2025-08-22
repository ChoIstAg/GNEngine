#include "GNEngine/component/SoundComponent.h"

#include <iostream>

void SoundComponent::addSound(const std::string& name, std::shared_ptr<Sound> sound, bool loop, float volume) {
    if (sounds_.find(name) == sounds_.end()) {
        SoundData data;
        data.resource = sound;
        data.loop = loop;
        data.volume = volume;
        sounds_[name] = data;
    }
}

void SoundComponent::play(const std::string& name) {
    auto it = sounds_.find(name);
    if (it != sounds_.end()) {
        it->second.wantsToPlay = true;
        it->second.wantsToStop = false;
    }
}

void SoundComponent::stop(const std::string& name) {
    auto it = sounds_.find(name);
    if (it != sounds_.end()) {
        it->second.wantsToPlay = false;
        it->second.wantsToStop = true;
    }
}

bool SoundComponent::isPlaying(const std::string& name) const {
    auto it = sounds_.find(name);
    if (it != sounds_.end()) {
        return it->second.sourceId.has_value();
    }
    return false;
}

SoundComponent::SoundData* SoundComponent::getSoundData(const std::string& name) {
    auto it = sounds_.find(name);
    if (it != sounds_.end()) {
        return &it->second;
    }
    return nullptr;
}

const SoundComponent::SoundData* SoundComponent::getSoundData(const std::string& name) const {
    auto it = sounds_.find(name);
    if (it != sounds_.end()) {
        return &it->second;
    }
    return nullptr;
}

std::unordered_map<std::string, SoundComponent::SoundData>& SoundComponent::getAllSounds() {
    return sounds_;
}

