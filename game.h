// COMP710 GP Framework
#ifndef __GAME_H
#define __GAME_H

#include <vector>
#include <memory>

#include "inputsystem.h"
#include "sprite.h"
// Forward declarations:
class Renderer;
class Scene;
class Game
{
	// Member methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	bool Initialise();
	bool DoGameLoop();
	void Quit();
	void ToggleDebugWindow();
	void SetCurrentScene(int index);
	int GetCurrentScene();
protected:
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void ProcessFrameCounting(float deltaTime);
	void DebugDraw();
private:
	Game();
	~Game();
	Game(const Game& game);
	Game& operator=(const Game& game);

	// Member data:
public:
protected:
	static Game* sm_pInstance;

	bool m_bShowDebugWindow;
	InputSystem* m_pInputSystem;
	Renderer* m_pRenderer;
	__int64 m_iLastTime;
	float m_fExecutionTime;
	float m_fElapsedSeconds;
	int m_iFrameCount;
	int m_iFPS;

	std::vector<Scene*> m_scenes;	
	int m_iCurrentScene;

#ifdef USE_LAG
	float m_fLag;
	int m_iUpdateCount;
#endif // USE_LAG
	bool m_bLooping;
private:
	Sprite* m_pBall;

	const int targetFPS = 120;
	const float targetFrameTime = 1.0f / targetFPS;
	const float stepSize = 1.0f / static_cast<float>(targetFPS);

};
#endif // __GAME_H