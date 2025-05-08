#ifndef PARTICLE_H
#define PARTICLE_H

#include "sprite.h"
#include "vector2.h"

class Particle {
public:
    Particle();
    void Initialise(Sprite* sprite);
    void Activate(Vector2 position);
    void Update(float deltaTime);
    void Draw(Renderer& renderer);
    bool IsAlive() const;

private:
    Sprite* m_pSprite = nullptr;
    Vector2 m_position;
    Vector2 m_velocity;
    float m_lifetime = 0.0f;
    float m_maxLifetime = 1.0f;
    bool m_active = false;
	float m_speedMultiplier = 1.0f;
};
#endif // PARTICLE_H