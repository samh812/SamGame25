#ifndef MACHINE_H
#define MACHINE_H

#include "entity.h"
#include "inputsystem.h"
#include "player.h"
#include "animatedsprite.h"
#include "sprite.h"
#include <vector>
#include <memory>

class Machine : public Entity {

public:
	Machine();
	virtual ~Machine();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	void SetPosition(const Vector2& position);
	Vector2 GetPosition() const;


	void Upgrade();
	void SetUpgradeArea(const Vector2& position, float width, float height);
	bool IsPlayerInUpgradeArea(Player* player);

	bool IsUpgraded() const;

	void AddUpgradeSprite(std::unique_ptr<Sprite> sprite);
	void AddAnimatedUpgradeSprite(std::unique_ptr<AnimatedSprite> sprite);

	int GetUpgradeLevel() const;
	void SetUpgradeLevel(int level);
	int GetNumUpgrades() const;

	int GetUpgradeCost() const;
	void SetUpgradeCosts(const std::vector<int>& costs);

	void SetValueIncrease(const std::vector<float>& values);

	virtual void SetSprite(Sprite* pSprite);
	virtual void SetAnimatedSprite(AnimatedSprite* pSprite);

	std::vector<Sprite*> GetUpgradeSprites() const;
	std::vector<AnimatedSprite*> GetAnimatedUpgradeSprites() const;

	float GetValueIncreases() const;
	float GetNextValueIncrease() const;

	void IncreaseValue(float value);

	void SetSharedSprite();


protected:

	bool m_bUpgraded;

private:
	Renderer* m_pRenderer;
	Vector2 m_position;

	Sprite* m_pSprite;
	AnimatedSprite* m_pAnimatedSprite;

	Vector2 m_upgradeAreaPosition;
	float m_upgradeAreaWidth;
	float m_upgradeAreaHeight;

	std::vector<std::unique_ptr<Sprite>> m_upgradeSprites;
	std::vector<std::unique_ptr<AnimatedSprite>> m_animatedUpgradeSprites;

	int m_upgradeLevel;
	int m_numUpgrades = 5;
	std::vector<int> m_upgradeCosts;
	std::vector<float> m_increases;

	float m_valueIncrease = 1.0f;
	bool m_ownsSprite = true;

};

#endif // MACHINE_H