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
    m_seekingTarget = false;
    m_maxLifetime = 10.0f; // still a max safety net

    switch (m_type) {
    case ParticleType::Coin:
        m_speedMultiplier = 0.03f;
        m_velocity = Vector2(
            (((rand() % 200) - 100) / 100.0f) * m_speedMultiplier,
            (((rand() % 200) - 100) / 100.0f) * m_speedMultiplier
        );
        m_targetPosition = Vector2(0.0f, 0.0f); // Or wherever your coin counter is
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

    m_lifetime += deltaTime;

    if (m_type == ParticleType::Coin) {
        if (m_lifetime < m_burstDuration) {
            // Initial burst movement
            m_position += m_velocity;
        }
        else {
            if (!m_seekingTarget) {
                m_seekingTarget = true;
                // Recalculate velocity toward target
                Vector2 direction = m_targetPosition - m_position;
                float length = direction.Length();
                //if (length > 0.01f)
                //    direction.Normalize();
                m_velocity = direction * 2.0f; // speed toward target
            }

            // Move toward target
            m_position += m_velocity * deltaTime;

            // Check if close enough to "die"
            if ((m_position - m_targetPosition).Length() < 5.0f) {
                m_active = false;
            }
        }
    }
    else {
        // Default behavior (e.g. spark)
        m_position += m_velocity * deltaTime;
        if (m_lifetime > m_maxLifetime) {
            m_active = false;
        }
    }
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