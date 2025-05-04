#include "machine.h"
#include "renderer.h"
#include "sprite.h"
#include "inlinehelpers.h"
#include "vector2.h"

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

