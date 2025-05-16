#include "player.h"
#include "renderer.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "inlinehelpers.h"
#include "vector2.h"
#include "xboxcontroller.h"

#include <algorithm>


Player::Player()
    : m_speed(400.0f)
{
}

Player::~Player()
{
	delete m_pAnimSprite;
	m_pAnimSprite = nullptr;
}

bool Player::Initialise(Renderer& renderer)
{

    m_pRenderer = &renderer;
    m_pAnimSprite = m_pRenderer->CreateAnimatedSprite("../assets/playerAnimSprite.png");
    m_pAnimSprite->SetupFrames(128, 128);
    m_pAnimSprite->SetLooping(true);
    m_pAnimSprite->SetFrameDuration(0.07f);
    m_pAnimSprite->Animate();
    m_pAnimSprite->SetScale(1.0f);

    if (m_pAnimSprite == nullptr)
    {
        return false;
    }

    m_position = Vector2(400.0f, 300.0f);
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

    //controller input
    XboxController* controller = inputSystem.GetController(0);
    if (controller != nullptr && controller->IsConnected())
    {
        Vector2 stick = controller->GetLeftStick();

        //normalise
        const float MAX_AXIS = 32768.0f;
        Vector2 controllerDir(stick.x / MAX_AXIS, stick.y / MAX_AXIS);

        //deadzone handling
        const float DEADZONE = 0.2f;
        if (std::abs(controllerDir.x) > DEADZONE || std::abs(controllerDir.y) > DEADZONE)
        {
            direction = controllerDir;
        }
    }


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

    const float spriteHalfWidth = m_pAnimSprite->GetWidth() / 2.0f;
    const float spriteHalfHeight = m_pAnimSprite->GetHeight() / 2.0f;

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

    m_pAnimSprite->Process(deltaTime);
}

void Player::Draw(Renderer& renderer)
{
    if (m_pAnimSprite && m_bAlive)
    {
        m_pAnimSprite->SetX(m_position.x);
        m_pAnimSprite->SetY(m_position.y);
        m_pAnimSprite->Draw(renderer);
    }
}

bool Player::IsKeyHeld(InputSystem& input, SDL_Scancode key)
{
	return input.GetKeyState(key) == BS_HELD;
}


void Player::AddMoney(int amount) {
	m_Shekels += amount;
	if (m_Shekels < 0) {
		m_Shekels = 0;
	}
}



bool Player::SpendMoney(int amount) {
	if (m_Shekels >= amount) {
		m_Shekels -= amount;
		return true;
	}
	return false;
}
