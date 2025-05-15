#include "moneybag.h"
#include <iostream>

MoneyBag::MoneyBag() 
    : m_value(0), m_active(false) {
}
MoneyBag::~MoneyBag()
{
    
}

void MoneyBag::SetValue(int value) {
    m_value = value;
}

int MoneyBag::GetValue() const {
    return m_value;
}

void MoneyBag::Activate(Vector2 position) {
    m_position = position;
    m_bAlive = true;
    m_active = true;
}

void MoneyBag::Deactivate() {
    m_bAlive = false;
    m_active = false;
}

bool MoneyBag::IsActive() const {
    return m_active;
}
void MoneyBag::SetSprite(Sprite* pSprite)
{
    m_pSprite = pSprite;
	m_bOwnsSprite = false; // Assume the sprite is managed elsewhere
}