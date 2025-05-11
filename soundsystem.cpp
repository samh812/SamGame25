#include "SoundSystem.h"
#include <iostream>

bool SoundSystem::Initialise() {
    FMOD_RESULT result = FMOD::System_Create(&m_system);
    if (result != FMOD_OK) return false;

    result = m_system->init(512, FMOD_INIT_NORMAL, nullptr);
    return result == FMOD_OK;
}

void SoundSystem::Update() {
    if (m_system) m_system->update();
}

void SoundSystem::Release() {
    for (auto& pair : m_sounds) {
        pair.second->release();
    }
    m_sounds.clear();
    if (m_system) {
        m_system->release();
        m_system = nullptr;
    }
}

bool SoundSystem::LoadSound(const std::string& name, const std::string& path, bool loop) {
    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
    FMOD::Sound* sound = nullptr;
    FMOD_RESULT result = m_system->createSound(path.c_str(), mode, nullptr, &sound);
    if (result != FMOD_OK) return false;
    m_sounds[name] = sound;
    return true;
}

void SoundSystem::PlaySound(const std::string& name) {
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        FMOD::Channel* channel = nullptr;
        m_system->playSound(it->second, nullptr, false, &channel);
        m_channels[name] = channel; // Store the channel
    }
}

void SoundSystem::StopSound(const std::string& name) {
    auto it = m_channels.find(name);
    if (it != m_channels.end() && it->second) {
        it->second->stop();
        m_channels.erase(it); // Optionally remove channel reference
    }
}