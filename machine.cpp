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

    // Clean up upgrade sprites
    //for (Sprite* sprite : m_upgradeSprites)
    //{
    //    delete sprite;
    //}
    //m_upgradeSprites.clear();
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
}

void Machine::Upgrade()
{
    //upgrade function
    if (m_pSprite)
    {


        if (m_upgradeLevel + 1 < static_cast<int>(m_upgradeSprites.size()))
        {
            ++m_upgradeLevel;
            m_pSprite = m_upgradeSprites[m_upgradeLevel];
            m_bUpgraded = true;
        }


    }
}

int Machine::GetUpgradeLevel() const
{
	return m_upgradeLevel;
}
int Machine::GetNumUpgrades() const
{
	return m_numUpgrades;
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

void Machine::SetUpgradeCosts(const std::vector<int>& costs) {
    m_upgradeCosts = costs;
}

int Machine::GetUpgradeCost() const {
    if (m_upgradeLevel < static_cast<int>(m_upgradeCosts.size())) {
        return m_upgradeCosts[m_upgradeLevel];
    }
    return 0;
}



void Machine::IncreaseValue(float value) {
    m_valueIncrease * value;
}



void Machine::SetValueIncrease(const std::vector<float>& values) {
    m_increases = values;
}

void Machine::SetSharedSprite() {
	m_ownsSprite = false; // Set to false to indicate that the sprite is shared
}

float Machine::GetValueIncreases() const {
    if (m_upgradeLevel >= 0 && m_upgradeLevel < static_cast<int>(m_increases.size())) {
        return m_increases[m_upgradeLevel];
    }
    return 0.0f;


}



