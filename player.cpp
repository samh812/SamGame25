#include "player.h"
#include "renderer.h"
#include "sprite.h"
#include "inlinehelpers.h"

Player::Player()
    : m_speed(150.0f)  // Pixels per second
{
}

Player::~Player()
{
}

bool Player::Initialise(Renderer& renderer)
{
    m_pSprite = renderer.CreateSprite("../assets/ball.png");
    if (m_pSprite == nullptr)
    {
        return false;
    }

    m_position = Vector2(400.0f, 300.0f);  // Start at center
    m_bAlive = true;
    return true;
}

void Player::Process(float deltaTime, InputSystem& inputSystem)
{
    m_velocity = Vector2(0.0f, 0.0f);

    if (inputSystem.GetKeyState(SDL_SCANCODE_W) == BS_HELD)
        m_velocity.y -= m_speed;
    if (inputSystem.GetKeyState(SDL_SCANCODE_S) == BS_HELD)
        m_velocity.y += m_speed;
    if (inputSystem.GetKeyState(SDL_SCANCODE_A) == BS_HELD)
        m_velocity.x -= m_speed;
    if (inputSystem.GetKeyState(SDL_SCANCODE_D) == BS_HELD)
        m_velocity.x += m_speed;

    m_position += m_velocity * deltaTime;
}

void Player::Draw(Renderer& renderer)
{
    if (m_pSprite && m_bAlive)
    {
        m_pSprite->SetX(m_position.x);
        m_pSprite->SetY(m_position.y);
        m_pSprite->Draw(renderer);
    }
}