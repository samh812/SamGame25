// COMP710 GP Framework
#ifndef __GAME_H
#define __GAME_H

#include "sprite.h"
// Forward declarations:
class Renderer;
class Game
{
	// Member methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	bool Initialise();
	bool DoGameLoop();
	void Quit();
protected:
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void ProcessFrameCounting(float deltaTime);
private:
	Game();
	~Game();
	Game(const Game& game);
	Game& operator=(const Game& game);

	// Member data:
public:
protected:
	static Game* sm_pInstance;

	Renderer* m_pRenderer;
	__int64 m_iLastTime;
	float m_fExecutionTime;
	float m_fElapsedSeconds;
	int m_iFrameCount;
	int m_iFPS;
#ifdef USE_LAG
	float m_fLag;
	int m_iUpdateCount;
#endif // USE_LAG
	bool m_bLooping;
private:
	Sprite* m_pBall;
};
#endif // __GAME_H