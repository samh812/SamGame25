#include "scenemainmenu.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include "game.h"
#include "renderer.h"
#include "texture.h"
#include "sprite.h"

SceneMainMenu::SceneMainMenu() : m_startPressed(false)
{
}

SceneMainMenu::~SceneMainMenu()
{
}

bool SceneMainMenu::Initialise(Renderer& renderer)
{


    return true;
}

void SceneMainMenu::Process(float deltaTime, InputSystem& inputSystem)
{

    // Example: Press RETURN to go to warehouse scene
    if (inputSystem.GetKeyState(SDL_SCANCODE_RETURN) == BS_PRESSED)
    {
        Game::GetInstance().SetCurrentScene(2); // Switch to SceneWarehouse
    }
    //m_pTitleText->Process(deltaTime);
}

void SceneMainMenu::Draw(Renderer& renderer)
{
    //m_pTitleText->Draw(renderer);
}

void SceneMainMenu::DebugDraw()
{
}