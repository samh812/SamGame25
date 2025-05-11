#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <fmod.hpp>
#include <string>
#include <map>

class SoundSystem
{
public:
    bool Initialise();
    void Update();
    void Release();
    bool LoadSound(const std::string& name, const std::string& path, bool loop = false);
    void PlaySound(const std::string& name);

private:
    FMOD::System* m_system = nullptr;
    std::map<std::string, FMOD::Sound*> m_sounds;
};

#endif 