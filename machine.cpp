#include "machine.h"
#include "renderer.h"
#include "sprite.h"
#include "inlinehelpers.h"
#include "vector2.h"
#include <iostream>

#include <algorithm>


Machine::Machine()

{
}

Machine::~Machine()
{
    delete m_pSprite;
    m_pSprite = nullptr;
}

bool Machine::Initialise(Renderer& renderer)
{
    m_pRenderer = &renderer;
    m_pSprite = m_pRenderer->CreateSprite("../assets/bottler_00.png");
    if (m_pSprite == nullptr)
    {
        return false;
    }

    m_position = Vector2(400.0f, 300.0f);
    m_pSprite->SetScale(0.2f);
    m_bAlive = true;
    return true;
}

void Machine::Process(float deltaTime, InputSystem& inputSystem)
{

}

void Machine::Draw(Renderer& renderer)
{
    if (m_pSprite && m_bAlive)
    {
        m_pSprite->SetX(m_position.x);
        m_pSprite->SetY(m_position.y);
        m_pSprite->Draw(renderer);
    }
}

void Machine::SetPosition(const Vector2& position)
{
    m_position = position;
}

Vector2 Machine::GetPosition() const
{
    return m_position;
}


bool Machine::IsPlayerInUpgradeArea(Player* player)
{
    // Get player position
    Vector2 playerPos = player->GetPosition();
    //std::cout << "Entity Position: x = " << playerPos.x << ", y = " << playerPos.y << std::endl;


    // Check if player is within the upgrade area
    bool isWithinX = playerPos.x >= m_upgradeAreaPosition.x && playerPos.x <= m_upgradeAreaPosition.x + m_upgradeAreaWidth;
    bool isWithinY = playerPos.y >= m_upgradeAreaPosition.y && playerPos.y <= m_upgradeAreaPosition.y + m_upgradeAreaHeight;

    return isWithinX && isWithinY;
}

void Machine::SetUpgradeArea(const Vector2& position, float width, float height)
{
    m_upgradeAreaPosition = position;
    m_upgradeAreaWidth = width;
    m_upgradeAreaHeight = height;

	//std::cout << "Upgrade area set to: " << m_upgradeAreaPosition.x << ", " << m_upgradeAreaPosition.y << ", " << m_upgradeAreaWidth << ", " << m_upgradeAreaHeight << std::endl;
}

void Machine::Upgrade()
{
    //upgrade function
    if (m_pSprite)
    {
		m_bUpgraded = true;
        //m_pSprite->SetTexture("../assets/updated_machine_sprite.png");
    }
}

bool Machine::IsUpgraded() const
{
    return m_bUpgraded;
}