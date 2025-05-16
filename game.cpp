// COMP710 GP Framework 2022
// 


#include <iostream>
// 
// This include:
#include "game.h"
#include "sprite.h"
// Library includes:
#include "renderer.h"
#include "logmanager.h"
#include "scenecheckerboards.h"
#include "scenebouncingballs.h"
#include "scenewarehouse.h"
#include "scenemainmenu.h"
#include "splashscreens.h"

#include "inputsystem.h"
#include <thread>
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"



// Static Members:
Game * Game::sm_pInstance = 0;
Game& Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}
	return (*sm_pInstance);
}
void Game::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}
Game::Game() : m_pRenderer(0), m_bLooping(true)
{
}
Game::~Game()
{
	delete m_pRenderer;
	m_pRenderer = nullptr;

	delete m_pInputSystem;
	m_pInputSystem = nullptr;

	for (Scene* scene : m_scenes)
	{
		delete scene;
		scene = nullptr;
	}
	m_scenes.clear();


}
void Game::Quit()
{
	m_bLooping = false;
}
bool Game::Initialise()
{


	m_pInputSystem = new InputSystem();
	if (!m_pInputSystem->Initialise()) {
		std::cerr << "Failed to initialize InputSystem." << std::endl;
		return false;
	}

	m_pRenderer = new Renderer();
	if (!m_pRenderer->Initialise(false, 0, 0))
	{
		LogManager::GetInstance().Log("Renderer failed to initialise!");
		return false;
	}

	int bbWidth = m_pRenderer->GetWidth();
	int bbHeight = m_pRenderer->GetHeight();


	m_iLastTime = SDL_GetPerformanceCounter();
	m_pRenderer->SetClearColour(0, 0, 0);


	Scene* pScene = new Splashscreens();
	pScene->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene);

	pScene = new SceneMainMenu();
	pScene->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene);

	pScene = new SceneWarehouse();
	pScene->Initialise(*m_pRenderer);
	m_scenes.push_back(pScene);


	m_iCurrentScene = 0;


	return true;
}
bool Game::DoGameLoop()
{


	m_pInputSystem->ProcessInput();

	if (m_bLooping)
	{
		Uint64 frameStart = SDL_GetPerformanceCounter();

		float deltaTime = (frameStart - m_iLastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
		m_iLastTime = frameStart;
		m_fExecutionTime += deltaTime;

		Process(deltaTime);

#ifdef USE_LAG
		m_fLag += deltaTime;
		int innerLag = 0;
		while (m_fLag >= stepSize)
		{
			Process(stepSize);
			m_fLag -= stepSize;
			++m_iUpdateCount;
			++innerLag;
		}
#endif // USE_LAG

		Draw(*m_pRenderer);

		//frame limiting
		Uint64 frameEnd = SDL_GetPerformanceCounter();
		float actualFrameTime = (frameEnd - frameStart) / static_cast<float>(SDL_GetPerformanceFrequency());
		float sleepTime = targetFrameTime - actualFrameTime;

		if (sleepTime > 0.0f)
		{
			SDL_Delay(static_cast<Uint32>(sleepTime * 1000.0f));
		}
	}
	return m_bLooping;
}
void Game::Process(float deltaTime)
{
	ProcessFrameCounting(deltaTime);

	ButtonState state1 = m_pInputSystem->GetKeyState(SDL_SCANCODE_SPACE);
	if (state1 == BS_PRESSED) {
		ToggleDebugWindow();
	}



	m_scenes[m_iCurrentScene]->Process(deltaTime, *m_pInputSystem);

}
void Game::Draw(Renderer& renderer)
{
	++m_iFrameCount;
	renderer.Clear();

	m_scenes[m_iCurrentScene]->Draw(renderer);
	DebugDraw();
	renderer.Present();

}
void
Game::ProcessFrameCounting(float deltaTime)
{
	// Count total simulation time elapsed:
	m_fElapsedSeconds += deltaTime;
	// Frame Counter:
	if (m_fElapsedSeconds > 1.0f)
	{
		m_fElapsedSeconds -= 1.0f;
		m_iFPS = m_iFrameCount;
		m_iFrameCount = 0;
	}
}

void Game::DebugDraw
()
{
	if (m_bShowDebugWindow)
	{
		bool open = true;
		ImGui::Begin("Debug Window", &open, ImGuiWindowFlags_MenuBar);
		ImGui::Text("The Last Bottler | COMP710 GP Framework (%s)", "2025, S1");

		ImGui::Text("Active scene: %d", m_iCurrentScene);
		m_scenes[m_iCurrentScene]->DebugDraw();
		if (ImGui::Button("Quit"))
		{
			Quit();
		}
		ImGui::End();
	}
}
void Game::ToggleDebugWindow
()
{
	m_bShowDebugWindow = !m_bShowDebugWindow;
	m_pInputSystem->ShowMouseCursor(m_bShowDebugWindow);
}

void Game::SetCurrentScene(int index)
{
	if (index >= 0 && index < static_cast<int>(m_scenes.size()) && index != m_iCurrentScene)
	{
		m_scenes[m_iCurrentScene]->OnExit();
		m_iCurrentScene = index;
		m_scenes[m_iCurrentScene]->OnEnter();
	}
}