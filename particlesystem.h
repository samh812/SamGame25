#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "particle.h"
#include <vector>

class ParticleSystem {
public:
    void Initialise(Sprite* sprite, int maxParticles = 50);
    void ActivateAt(Vector2 position);
    void Update(float deltaTime);
    void Draw(Renderer& renderer);
    bool IsFinished() const;

private:
    std::vector<Particle> m_particles;
    Sprite* m_pSprite = nullptr;
};

#endif // PARTICLESYSTEM_H
