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
    delete m_pMainMenuSprite;
    m_pMainMenuSprite = nullptr;
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

    m_pMainMenuSprite = renderer.CreateSprite("../assets/mainMenu.png");
    float scaleX = static_cast<float>(renderer.GetWidth()) / m_pMainMenuSprite->GetWidth();
    float scaleY = static_cast<float>(renderer.GetHeight()) / m_pMainMenuSprite->GetHeight();
    float scale = std::max(scaleX, scaleY);  //ensuring background covers whole screen

    m_pMainMenuSprite->SetX(renderer.GetWidth() / 2);
    m_pMainMenuSprite->SetY(renderer.GetHeight() / 2);
    m_pMainMenuSprite->SetScale(scale);

    return true;
}

void SceneMainMenu::Process(float deltaTime, InputSystem& inputSystem)
{
    m_pMainMenuSprite->Process(deltaTime);
    if (m_menuActive) {

        XboxController* controller = inputSystem.GetController(0);
        bool controllerA = false;
        if (controller != nullptr) {
            controllerA = controller->GetButtonState(SDL_CONTROLLER_BUTTON_A) == BS_PRESSED;
        }
        if (inputSystem.GetKeyState(SDL_SCANCODE_RETURN) == BS_PRESSED || controllerA)
        {
            Game::GetInstance().SetCurrentScene(2);
        }

        bool controllerB = false;
        if (controller != nullptr) {
            controllerB = controller->GetButtonState(SDL_CONTROLLER_BUTTON_B) == BS_PRESSED;
        }
        if (inputSystem.GetKeyState(SDL_SCANCODE_ESCAPE) == BS_PRESSED || controllerB)
        {
            Game::GetInstance().Quit();
        }

    }

}

void SceneMainMenu::Draw(Renderer& renderer)
{
    m_pMainMenuSprite->Draw(renderer);
}

void SceneMainMenu::DebugDraw()
{
}