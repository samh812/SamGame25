#include "particle.h"
#include <cstdlib>
#include <iostream>

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
    m_maxLifetime = 1.0f;

    switch (m_type) {
    case ParticleType::Coin:
        m_speedMultiplier = 15.0f;
        m_velocity = Vector2(
            (((rand() % 200) - 100) / 100.0f) * m_speedMultiplier,
            (((rand() % 200) - 100) / 100.0f) * m_speedMultiplier
        );
        m_maxLifetime = 3.2f;
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

    if (m_pPlayer) {
        m_targetPosition = m_pPlayer->GetPosition();
    }

    if (m_type == ParticleType::Coin) {
        if (m_lifetime < m_burstDuration) {
            //initial burst movement
            m_position += m_velocity;
            m_velocity *= 0.92f; //friction
        }
        else { //converge on player
            if (!m_seekingTarget) {
                m_seekingTarget = true;
            }

            //calculate direction to target
            Vector2 toTarget = m_targetPosition - m_position;


            float length = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

            //deactivate if close enough
            float deactivateDistance = 150.0f;
            if (length < deactivateDistance) {
                m_active = false;
                return;
            }

            if (length > 0.0f) {
                Vector2 direction;
                direction.x = toTarget.x / length;
                direction.y = toTarget.y / length;

                float minDistance = 0.1f;
                float distanceFactor = std::max(length, minDistance);
                float acceleration = 3000.0f;
                m_velocity.x += direction.x * acceleration * deltaTime;
                m_velocity.y += direction.y * acceleration * deltaTime;


                m_position.x += m_velocity.x * deltaTime;
                m_position.y += m_velocity.y * deltaTime;
            }


            // Deactivate after lifetime expires
            if (m_lifetime > m_maxLifetime) {
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

void Particle::SetTargetPosition(Vector2 target) {
    m_targetPosition = target;
}
void Particle::SetPlayer(Player* player) {
    m_pPlayer = player;
}