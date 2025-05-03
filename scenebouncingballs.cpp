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
#include "animatedsprite.h"
#include "inputsystem.h"


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

	delete m_pTestSprite;
	delete m_pAnimSprite1;
	delete m_pAnimSprite2;
	delete m_pWelcomeText;

	m_pTestSprite = nullptr;
	m_pAnimSprite1 = nullptr;
	m_pAnimSprite2 = nullptr;
	m_pWelcomeText = nullptr;
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






	m_pAnimSprite1 = renderer.CreateAnimatedSprite("../assets/anim8strip.png");
	m_pAnimSprite1->SetupFrames(64, 64); // 8 frames, 1 row
	m_pAnimSprite1->SetLooping(true);
	m_pAnimSprite1->SetFrameDuration(0.1f);
	m_pAnimSprite1->Animate();
	m_pAnimSprite1->SetScale(1.0f);

	m_pAnimSprite2 = renderer.CreateAnimatedSprite("../assets/explosion.png");
	m_pAnimSprite2->SetupFrames(64, 64); // 8 frames, 1 row
	m_pAnimSprite2->SetLooping(true);
	m_pAnimSprite2->SetFrameDuration(0.1f);
	m_pAnimSprite2->Animate();
	m_pAnimSprite2->SetScale(1.0f);

	m_pAnimSprite1->SetX(renderer.GetWidth() / 2);
	m_pAnimSprite1->SetY(renderer.GetHeight() / 2);


	m_pAnimSprite2->SetX((renderer.GetWidth() / 2) + 100);
	m_pAnimSprite2->SetY(renderer.GetHeight() / 2);


	// Load static text textures into the Texture Manager...
	renderer.CreateStaticText("Auckland University of Technology", 60);
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

	ButtonState leftArrow = inputSystem.GetKeyState(SDL_SCANCODE_LEFT);
	if (leftArrow == BS_PRESSED || leftArrow == BS_HELD) {
		m_pAnimSprite1->SetX((m_pAnimSprite1->GetX()) - 1);
	}
	ButtonState rightArrow = inputSystem.GetKeyState(SDL_SCANCODE_RIGHT);
	if (rightArrow == BS_PRESSED || rightArrow == BS_HELD) {
		m_pAnimSprite1->SetX((m_pAnimSprite1->GetX()) + 1);
	}

	ButtonState upArrow = inputSystem.GetKeyState(SDL_SCANCODE_UP);
	if (upArrow == BS_PRESSED || upArrow == BS_HELD) {
		m_pAnimSprite1->SetY((m_pAnimSprite1->GetY()) - 1);
	}
	ButtonState downArrow = inputSystem.GetKeyState(SDL_SCANCODE_DOWN);
	if (downArrow == BS_PRESSED || downArrow == BS_HELD) {
		m_pAnimSprite1->SetY((m_pAnimSprite1->GetY()) + 1);
	}

	playerMovement();

	m_pAnimSprite1->Process(deltaTime);
	m_pAnimSprite2->Process(deltaTime);
	//m_pAnimSprite3->Process(deltaTime);

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

	 //Set positions for animated sprites

	m_pAnimSprite1->Draw(renderer);
	m_pAnimSprite2->Draw(renderer);

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

void SceneBouncingBalls::playerMovement() {

	m_pAnimSprite1->GetY();


	//ButtonState quitButton = m_pInputSystem->GetKeyState(SDL_SCANCODE_ESCAPE);
	//if (quitButton == BS_PRESSED) {
	//	Quit();
	//}
}
