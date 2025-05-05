#include "machine.h"
#include "renderer.h"
#include "sprite.h"
#include "inlinehelpers.h"
#include "vector2.h"
#include "texture.h"
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
    //get player position
    Vector2 playerPos = player->GetPosition();
    //std::cout << "Entity Position: x = " << playerPos.x << ", y = " << playerPos.y << std::endl;


    //check if player is within the upgrade area
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


        if (m_upgradeLevel + 1 < static_cast<int>(m_upgradeSprites.size()))
        {
			std::cout << "Upgrade lvl before: " << m_upgradeLevel << std::endl;
            ++m_upgradeLevel;
            std::cout << "Upgrade lvl after: " << m_upgradeLevel << std::endl;

            m_pSprite = m_upgradeSprites[m_upgradeLevel];
            m_bUpgraded = true;
        }
    }
}

int Machine::GetUpgradeLevel() const
{
	return m_upgradeLevel;
}

bool Machine::IsUpgraded() const
{
    return m_bUpgraded;
}

void Machine::AddUpgradeSprite(Sprite* sprite)
{
	m_upgradeSprites.push_back(sprite);
}

void Machine::SetSprite(Sprite* pSprite)
{
    m_pSprite = pSprite;
}

const std::vector<Sprite*>& Machine::GetUpgradeSprites() const
{
    return m_upgradeSprites;
}