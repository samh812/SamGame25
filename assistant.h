#ifndef ASSISTANT_H
#define ASSISTANT_H


#include "sprite.h"
#include "player.h"
#include "moneybag.h"
#include "soundsystem.h"

#include <vector>

enum class AssistantState
{
    Locked,
    Idle,
    Searching,
    MovingToBag,
    Collecting
};

class Assistant
{
public:
    Assistant();
    ~Assistant();

    void Initialise(Sprite* sprite, Player* player, float screenWidth, float screenHeight);
    void Update(float deltaTime, std::vector<MoneyBag*>& moneyBags);
    void Draw(Renderer& renderer);

    void Unlock(); //player buys the assistant
    bool IsUnlocked() const;

    bool IsPlayerInAssistantArea(Player* player);

private:
    SoundSystem m_soundSystem;
    AssistantState m_state;
    Sprite* m_pSprite;
    Player* m_pPlayer;
    Vector2 m_position;
    Vector2 m_targetPos;
    MoneyBag* m_targetBag;

    float m_speed = 150.0f;

    float m_searchCooldown = 0.0f; //time until next moneybag search
    float m_searchInterval = 1.0f;

    void FindNearestMoneyBag(const std::vector<MoneyBag*>& moneyBags);
    void MoveTowards(float deltaTime, const Vector2& target);
};

#endif