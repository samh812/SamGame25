#ifndef MACHINE_H
#define MACHINE_H

#include "entity.h"
#include "inputsystem.h"

class Machine : public Entity {

public:
	Machine();
	virtual ~Machine();

	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);

	void SetPosition(const Vector2& position);  // Add this setter function
	Vector2 GetPosition() const;                // Optional: to retrieve position


private:
	Renderer* m_pRenderer;  // Pointer to the Renderer object
	Vector2 m_position;
};

#endif // MACHINE_H