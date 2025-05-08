#include "particle.h"
#include <cstdlib>

Particle::Particle() {}

void Particle::Initialise(Sprite* sprite, ParticleType type) {
    m_pSprite = sprite;
    m_type = type;
}

void Particle::Activate(Vector2 position) {
    m_position = position;
    m_lifetime = 0.0f;
    m_active = true;
    m_maxLifetime = 10.0f;

    switch (m_type) {
    case ParticleType::Coin:
        m_speedMultiplier = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (3.0f - 1.0f)));
        m_velocity = Vector2(
            (((rand() % 200) - 100) / 100.0f) * m_speedMultiplier,
            (((rand() % 200) - 100) / 100.0f) * m_speedMultiplier
        );
        break;

    case ParticleType::Spark:
        m_speedMultiplier = 2.0f;
        m_velocity = Vector2(
            (((rand() % 200) - 100) / 100.0f) * m_speedMultiplier,
            -std::abs(((rand() % 100) / 100.0f) * m_speedMultiplier) // sparks go upward
        );
        m_maxLifetime = 0.5f;
        break;
    }
}

void Particle::Update(float deltaTime) {
    if (!m_active) return;
    m_position += m_velocity * deltaTime * 100;
    m_lifetime += deltaTime;
    if (m_lifetime >= m_maxLifetime) m_active = false;
}

void Particle::Draw(Renderer& renderer) {
    if (!m_active || !m_pSprite) return;
    m_pSprite->SetX(m_position.x);
    m_pSprite->SetY(m_position.y);
    m_pSprite->Draw(renderer);
}

bool Particle::IsAlive() const {
    return m_active;
}