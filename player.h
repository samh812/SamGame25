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
	bool IsKeyHeld(InputSystem& input, SDL_Scancode key);

private:
	float m_speed;
	Renderer* m_pRenderer;  // Pointer to the Renderer object
};

#endif // PLAYER_H