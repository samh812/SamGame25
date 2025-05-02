// COMP710 GP Framework
#ifndef SCENEBOUNCINGBALLS_H
#define SCENEBOUNCINGBALLS_H
// Local includes:
#include "scene.h"
#include "sprite.h"
#include "inputsystem.h"

// Forward declarations:
class Renderer;
class Ball;
class AnimatedSprite;
// Class declaration:
class SceneBouncingBalls : public Scene
{

	//other members:
	// 
	
private:
	AnimatedSprite* m_pAnimSprite1;
	AnimatedSprite* m_pAnimSprite2;
	AnimatedSprite* m_pAnimSprite3;
	// Member methods:
public:
	SceneBouncingBalls();
	virtual ~SceneBouncingBalls();
	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();
	void playerMovement();
protected:
private:
	SceneBouncingBalls(const SceneBouncingBalls& sceneBouncingBalls);
	SceneBouncingBalls& operator=(const SceneBouncingBalls& sceneBouncingBalls);
	// Member data:
public:

protected:
	Ball* m_pBalls[100];
	int m_iShowCount;

private:
	Sprite* m_pTestSprite;
	Sprite* m_pWelcomeText;

};
#endif // SCENEBOUNCINGBALLS_H
