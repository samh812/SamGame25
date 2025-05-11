#include "scenemainmenu.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include "game.h"
#include "renderer.h"
#include "texture.h"
#include "sprite.h"
#include "xboxcontroller.h"

SceneMainMenu::SceneMainMenu() : m_menuActive(true)
{
}

SceneMainMenu::~SceneMainMenu()
{
}
void SceneMainMenu::OnEnter() {
    m_menuActive = true;

};

void SceneMainMenu::OnExit()
{
    m_menuActive = false;
}
bool SceneMainMenu::Initialise(Renderer& renderer)
{


    return true;
}

void SceneMainMenu::Process(float deltaTime, InputSystem& inputSystem)
{

    if (m_menuActive) {

        if (inputSystem.GetKeyState(SDL_SCANCODE_RETURN) == BS_PRESSED)
        {
            Game::GetInstance().SetCurrentScene(2);
        }
        XboxController* controller = inputSystem.GetController(0);
        if (controller != nullptr) {
            if (controller->GetButtonState(SDL_CONTROLLER_BUTTON_A) == BS_PRESSED) {
                Game::GetInstance().SetCurrentScene(2);
            }
        }
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