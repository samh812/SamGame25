// COMP710 Steffan Hooper
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
// This include:
#include "scenebouncingballs.h"
// Local includes:
#include "renderer.h"
#include "ball.h"
#include "sprite.h"

#include "imgui/imgui.h"
// Library includes:
#include <cassert>
#include <cstdlib>
SceneBouncingBalls::SceneBouncingBalls()
	: m_pBalls{ 0 }
	, m_iShowCount(0)
{
}
SceneBouncingBalls::~SceneBouncingBalls()
{
	for (int k = 0; k < 10; ++k)
	{
		delete m_pBalls[k];
		m_pBalls[k] = 0;
	}
}
bool
SceneBouncingBalls::Initialise(Renderer& renderer)
{
	for (int k = 0; k < 10; ++k)
	{
		m_pBalls[k] = new Ball();
		m_pBalls[k]->Initialise(renderer);
		m_pBalls[k]->RandomiseColour();
	}
	// Always place one ball at the centre...
	m_pBalls[0]->Position().x = renderer.GetWidth() / 2.0f;
	m_pBalls[0]->Position().y = renderer.GetHeight() / 2.0f;
	m_iShowCount = 10;


	m_pTestSprite = renderer.CreateSprite("../assets/ball.png");




	// Load static text textures into the Texture Manager...
	renderer.CreateStaticText("Auckland University of Technology", 60);
	// Generate sprites that use the static text textures...
	m_pWelcomeText = renderer.CreateSprite("Auckland University of Technology");
	m_pWelcomeText->SetY(200);
	m_pWelcomeText->SetX(500);


	return true;
}
void
SceneBouncingBalls::Process(float deltaTime, InputSystem& inputSystem)
{
	for (int k = 0; k < m_iShowCount; ++k)
	{
		m_pBalls[k]->Process(deltaTime);
	}
	m_pWelcomeText->Process(deltaTime);

}
void
SceneBouncingBalls::Draw(Renderer& renderer)
{


	// Draw the test sprite
	if (m_pTestSprite)
	{
		m_pTestSprite->Draw(renderer);
	}
	for (int k = 0; k < m_iShowCount; ++k)
	{
		m_pBalls[k]->Draw(renderer);
	}
	m_pWelcomeText->Draw(renderer);
}

void SceneBouncingBalls::DebugDraw
()
{
	ImGui::Text("Scene: Bouncing Balls");
	ImGui::SliderInt("Show Count", &m_iShowCount, 1, 10);
	static int editBallNumber = 0;
	ImGui::SliderInt("Edit ball", &editBallNumber, 0, 9);
	m_pBalls[editBallNumber]->DebugDraw();
}