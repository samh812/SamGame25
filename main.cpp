// COMP710 GP Framework 2022
// Library includes:
#include <SDL.h>
#include <crtdbg.h>

// Local includes:
#include "game.h"
#include "logmanager.h"



int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//_CrtSetBreakAlloc(337);
	Game& gameInstance = Game::GetInstance();
	if (!gameInstance.Initialise())
	{
		LogManager::GetInstance().Log("Game initialise failed!");
		return 1;
	}
	while (gameInstance.DoGameLoop())
	{
		// No body.
	}

	SDL_Quit();

	Game::DestroyInstance();
	LogManager::DestroyInstance();
	//_CrtDumpMemoryLeaks();
	return 0;
}
