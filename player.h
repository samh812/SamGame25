#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "inputsystem.h"
#include "particle.h"

class Player : public Entity {

public:
	Player();
	~Player();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime, InputSystem& inputSystem);
	void Draw(Renderer& renderer);
	bool IsKeyHeld(InputSystem& input, SDL_Scancode key);


	void AddMoney(int amount);
	bool SpendMoney(int amount);
	int GetMoney() { return m_Shekels; };
	Vector2 GetPosition() const { return m_position; };


private:
	float m_speed;
	Renderer* m_pRenderer;  // Pointer to the Renderer object
	int m_Shekels;
};

#endif // PLAYER_H