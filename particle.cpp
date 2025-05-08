#include "particle.h"
#include <cstdlib>

Particle::Particle() {}

void Particle::Initialise(Sprite* sprite) {
    m_pSprite = sprite;
}

void Particle::Activate(Vector2 position) {
    m_position = position;
    m_velocity = Vector2(((rand() % 200) - 100) / 100.0f, ((rand() % 200) - 100) / 100.0f);
    m_lifetime = 0.0f;
    m_maxLifetime = 1.0f;
    m_active = true;
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