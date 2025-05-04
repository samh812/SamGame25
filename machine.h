#ifndef MACHINE_H
#define MACHINE_H

#include "entity.h"
#include "inputsystem.h"
#include "player.h"

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


private:
	Renderer* m_pRenderer;
	Vector2 m_position;

	Vector2 m_upgradeAreaPosition;
	float m_upgradeAreaWidth;
	float m_upgradeAreaHeight;
	bool m_bUpgraded;
};

#endif // MACHINE_H