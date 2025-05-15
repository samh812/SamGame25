#include "machine.h"
#include "renderer.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "inlinehelpers.h"
#include "scenewarehouse.h"
#include "vector2.h"
#include "texture.h"
#include <iostream>

#include <algorithm>


Machine::Machine()

{
}
Machine::~Machine()
{
    //delete m_pSprite;
    //m_pSprite = nullptr;

}


bool Machine::Initialise(Renderer& renderer)
{
    return true;
}

void Machine::Process(float deltaTime, InputSystem& inputSystem)
{
    if (m_pAnimatedSprite) {
        m_pAnimatedSprite->SetFrameDuration(GetValueIncreases());
        m_pAnimatedSprite->Process(deltaTime);

    }
}

void Machine::Draw(Renderer& renderer)
{
    if (m_pAnimatedSprite && m_bAlive)
    {
        m_pAnimatedSprite->SetX(m_position.x);
        m_pAnimatedSprite->SetY(m_position.y);
        m_pAnimatedSprite->Draw(renderer);
    }
    else if (m_pSprite && m_bAlive)
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
    if (!m_animatedUpgradeSprites.empty())
    {
        if (m_upgradeLevel + 1 < static_cast<int>(m_animatedUpgradeSprites.size()))
        {
            ++m_upgradeLevel;
            m_pAnimatedSprite = m_animatedUpgradeSprites[m_upgradeLevel].get();
            m_bUpgraded = true;
        }
    }
    else if (!m_upgradeSprites.empty())
    {
        if (m_upgradeLevel + 1 < static_cast<int>(m_upgradeSprites.size()))
        {
            ++m_upgradeLevel;
            m_pSprite = m_upgradeSprites[m_upgradeLevel].get();
            m_bUpgraded = true;
        }
    }
}

int Machine::GetUpgradeLevel() const
{
	return m_upgradeLevel;
}

void Machine::SetUpgradeLevel(int level) {
    m_upgradeLevel = level;
}
int Machine::GetNumUpgrades() const
{
	return m_numUpgrades;
}

bool Machine::IsUpgraded() const
{
    return m_bUpgraded;
}

void Machine::AddUpgradeSprite(std::unique_ptr<Sprite> sprite)
{
    m_upgradeSprites.push_back(std::move(sprite));
}
void Machine::AddAnimatedUpgradeSprite(std::unique_ptr<AnimatedSprite> sprite)
{
    m_animatedUpgradeSprites.push_back(std::move(sprite));
}

void Machine::SetSprite(Sprite* pSprite)
{
    m_pSprite = pSprite;
}
void Machine::SetAnimatedSprite(AnimatedSprite* pSprite)
{
    m_pAnimatedSprite = pSprite;
}

std::vector<Sprite*> Machine::GetUpgradeSprites() const
{
    std::vector<Sprite*> result;
    result.reserve(m_upgradeSprites.size());
    for (const auto& uptr : m_upgradeSprites)
    {
        result.push_back(uptr.get());
    }
    return result;
}

std::vector<AnimatedSprite*> Machine::GetAnimatedUpgradeSprites() const
{
    std::vector<AnimatedSprite*> result;
    result.reserve(m_animatedUpgradeSprites.size());
    for (const auto& uptr : m_animatedUpgradeSprites)
    {
        result.push_back(uptr.get());
    }
    return result;
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


float Machine::GetNextValueIncrease() const {
    if (m_upgradeLevel >= 0 && m_upgradeLevel < static_cast<int>(m_increases.size())-1) {
        return m_increases[m_upgradeLevel+1];
    }
    return 0.0f;


}




