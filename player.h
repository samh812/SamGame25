#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "inputsystem.h"

class Player : public Entity {

public:
	Player();
	~Player();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime, InputSystem& inputSystem);
	void Draw(Renderer& renderer);

private:
	float m_speed;
};

#endif // PLAYER_H