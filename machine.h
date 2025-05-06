#ifndef MACHINE_H
#define MACHINE_H

#include "entity.h"
#include "inputsystem.h"
#include "player.h"
#include <vector>

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

	void AddUpgradeSprite(Sprite* sprite);
	int GetUpgradeLevel() const;
	int GetNumUpgrades() const;

	int GetUpgradeCost() const;
	void SetUpgradeCosts(const std::vector<int>& costs);


	virtual void SetSprite(Sprite* pSprite);
	const std::vector<Sprite*>& GetUpgradeSprites() const;



protected:

	bool m_bUpgraded;

private:
	Renderer* m_pRenderer;
	Vector2 m_position;

	Vector2 m_upgradeAreaPosition;
	float m_upgradeAreaWidth;
	float m_upgradeAreaHeight;

	std::vector<Sprite*> m_upgradeSprites;
	int m_upgradeLevel;
	int m_numUpgrades = 2;
	std::vector<int> m_upgradeCosts;
};

#endif // MACHINE_H