#include "player.h"
#include "renderer.h"
#include "sprite.h"
#include "inlinehelpers.h"
#include "vector2.h"

#include <algorithm>


Player::Player()
    : m_speed(400.0f)
{
}

Player::~Player()
{
	delete m_pSprite;
	m_pSprite = nullptr;
}

bool Player::Initialise(Renderer& renderer)
{
    m_pRenderer = &renderer;
    m_pSprite = m_pRenderer->CreateSprite("../assets/ball.png");
    if (m_pSprite == nullptr)
    {
        return false;
    }

    m_position = Vector2(400.0f, 300.0f);
    m_pSprite->SetScale(0.2f);
    m_bAlive = true;
    return true;
}

void Player::Process(float deltaTime, InputSystem& inputSystem)
{
    Vector2 direction(0.0f, 0.0f);

    if (IsKeyHeld(inputSystem, SDL_SCANCODE_W) || IsKeyHeld(inputSystem, SDL_SCANCODE_UP))    direction.y -= 1.0f;
    if (IsKeyHeld(inputSystem, SDL_SCANCODE_S) || IsKeyHeld(inputSystem, SDL_SCANCODE_DOWN))  direction.y += 1.0f;
    if (IsKeyHeld(inputSystem, SDL_SCANCODE_A) || IsKeyHeld(inputSystem, SDL_SCANCODE_LEFT))  direction.x -= 1.0f;
    if (IsKeyHeld(inputSystem, SDL_SCANCODE_D) || IsKeyHeld(inputSystem, SDL_SCANCODE_RIGHT)) direction.x += 1.0f;

    //normalising to prevent faster diagonal movement
    if (direction.x != 0.0f || direction.y != 0.0f)
    {
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction.x /= length;
        direction.y /= length;
    }

    //move the player
    m_position += direction * m_speed * deltaTime;

    //clamp to screen with halfWidth, to prevent clipping outside screen
    const int screenWidth = m_pRenderer->GetWidth();
    const int screenHeight = m_pRenderer->GetHeight();

    const float spriteHalfWidth = m_pSprite->GetWidth() / 2.0f;
    const float spriteHalfHeight = m_pSprite->GetHeight() / 2.0f;

    float wallMarginX = screenWidth * 0.02f;  //2% horizontal margin (for the walls)
    float wallMarginY = screenHeight * 0.02f; //2% vertical margin

    float minX = wallMarginX + spriteHalfWidth;
    float maxX = screenWidth - wallMarginX - spriteHalfWidth;
    float minY = wallMarginY + spriteHalfHeight;
    float machineY = screenHeight * 0.85f; //machines are placed at 85% height
    float machineTopY = machineY - (160.0f / 2.0f); //machines are always 160

    float maxY = std::min(screenHeight - wallMarginY - spriteHalfHeight, machineTopY - spriteHalfHeight);


    m_position.x = std::max(minX, std::min(maxX, m_position.x));
    m_position.y = std::max(minY, std::min(maxY, m_position.y));
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

bool Player::IsKeyHeld(InputSystem& input, SDL_Scancode key)
{
	return input.GetKeyState(key) == BS_HELD;
}