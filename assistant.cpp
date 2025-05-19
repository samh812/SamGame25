#include "assistant.h"
#include "sprite.h"
#include "moneybag.h"
#include "player.h"
#include "scenewarehouse.h"

#include <fmod.hpp>
#include <fmod_errors.h>

#include <cmath>
#include <iostream>

Assistant::Assistant()
    : m_state(AssistantState::Locked), m_pSprite(nullptr), m_pPlayer(nullptr), m_targetBag(nullptr)
{
}

Assistant::~Assistant()
{
    if (m_pSprite)
    {
        delete m_pSprite;
        m_pSprite = nullptr;
    }

}

void Assistant::Initialise(Sprite* sprite, Player* player, float screenWidth, float screenHeight)
{
    m_pSprite = sprite;
    m_pPlayer = player;

    m_position = Vector2(static_cast<float>(sprite->GetX()), static_cast<float>(sprite->GetY()));


    if (!m_soundSystem.Initialise()) {
        std::cerr << "Failed to initialise FMOD system!" << std::endl;

    }
    m_soundSystem.LoadSound("assistantpickup", "../assets/sounds/coin.wav");


}

void Assistant::Unlock()
{
    m_state = AssistantState::Idle;
}

void Assistant::Lock()
{
    m_state = AssistantState::Locked;
}

bool Assistant::IsUnlocked() const
{
    return m_state != AssistantState::Locked;
}

void Assistant::Update(float deltaTime, std::vector<MoneyBag*>& moneyBags)
{
    if (m_state == AssistantState::Locked)
        return;

    //update position/state
    switch (m_state)
    {
    case AssistantState::Idle:
        m_state = AssistantState::Searching;
        break;

    case AssistantState::Searching:
        //only search if cooldown has expired
        m_searchCooldown -= deltaTime;
        if (m_searchCooldown <= 0.0f)
        {
            FindNearestMoneyBag(moneyBags);
            m_searchCooldown = m_searchInterval;

            if (m_targetBag)
            {
                m_targetPos = m_targetBag->GetPosition();
                m_state = AssistantState::MovingToBag;
            }
        }
        break;

    case AssistantState::MovingToBag:
        MoveTowards(deltaTime, m_targetPos);

        //recheck target every few seconds
        m_searchCooldown -= deltaTime;
        if (m_searchCooldown <= 0.0f)
        {
            FindNearestMoneyBag(moneyBags);
            m_searchCooldown = m_searchInterval;

            if (m_targetBag)
            {
                m_targetPos = m_targetBag->GetPosition();
            }
        }

        if ((m_position - m_targetPos).Length() < 20.0f)
        {
            m_state = AssistantState::Collecting;
        }
        break;

    case AssistantState::Collecting:
        if (m_targetBag && m_targetBag->IsActive())
        {
            m_pPlayer->AddMoney(m_targetBag->GetValue());
            m_soundSystem.PlaySound("assistantpickup");
            m_targetBag->Deactivate();
        }
        m_targetBag = nullptr;
        m_state = AssistantState::Idle;
        break;

    default:
        break;
    }

    if (m_pSprite)
    {
        m_pSprite->SetX(static_cast<int>(m_position.x));
        m_pSprite->SetY(static_cast<int>(m_position.y));
    }
}

void Assistant::Draw(Renderer& renderer)
{
    if (m_pSprite)
    {
        m_pSprite->Draw(renderer);
    }
}

void Assistant::FindNearestMoneyBag(const std::vector<MoneyBag*>& moneyBags)
{
    float closestDist = std::numeric_limits<float>::max();
    m_targetBag = nullptr;


    for (MoneyBag* pBag : moneyBags)
    {
        if (pBag->IsActive())
        {
            Vector2 bagPos = pBag->GetPosition();
            float dx = bagPos.x - m_position.x;
            float dy = bagPos.y - m_position.y;
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist < closestDist)
            {
                closestDist = dist;
                m_targetBag = pBag;
            }
        }
    }
}

void Assistant::MoveTowards(float deltaTime, const Vector2& target)
{
    Vector2 direction = target - m_position;
    float distance = direction.Length();

    if (distance > 0.0f)
    {
        direction /= distance;

        //apply easing: slow down when near the target
        float speed = m_speed;
        if (distance < 100.0f)  //start slowing down
            speed *= (distance / 100.0f);

        m_position.x += direction.x * speed * deltaTime;
        m_position.y += direction.y * speed * deltaTime;
    }

}



bool Assistant::IsPlayerInAssistantArea(Player* player)
{
    if (!m_pSprite || !player)
        return false;

    Vector2 playerPos = player->GetPosition();

    float assistantX = static_cast<float>(m_pSprite->GetX());
    float assistantY = static_cast<float>(m_pSprite->GetY());

    float areaWidth = 128.0f;
    float areaHeight = 128.0f;

    bool isWithinX = playerPos.x >= assistantX - areaWidth / 2 &&
        playerPos.x <= assistantX + areaWidth / 2;

    bool isWithinY = playerPos.y >= assistantY - areaHeight / 2 &&
        playerPos.y <= assistantY + areaHeight / 2;

    return isWithinX && isWithinY;
}


Vector2 Assistant::GetAssistantPosition() {
    return m_position;
}
