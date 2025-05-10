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

    //renderer.CreateStaticText("Auckland University of Technology", 60);
    //m_pTitleText = renderer.CreateSprite("Auckland University of Technology");
    //m_pTitleText->SetY(200);
    //m_pTitleText->SetX(500);

    //float scaleX = static_cast<float>(renderer.GetWidth()) / m_pTitleText->GetWidth();
    //float scaleY = static_cast<float>(renderer.GetHeight()) / m_pTitleText->GetHeight();
    //float scale = std::max(scaleX, scaleY);  //ensuring background covers whole screen

    //m_pTitleText->SetX(renderer.GetWidth() / 2);
    //m_pTitleText->SetY(renderer.GetHeight() / 2);
    //m_pTitleText->SetScale(scale);
    return true;
}

void SceneMainMenu::Process(float deltaTime, InputSystem& inputSystem)
{
    if (m_startPressed)
    {
        Game::GetInstance().SetCurrentScene(1);
    }
    //m_pTitleText->Process(deltaTime);
}

void SceneMainMenu::Draw(Renderer& renderer)
{
    //m_pTitleText->Draw(renderer);
}

void SceneMainMenu::DebugDraw()
{
    ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Welcome to the Game!");
    if (ImGui::Button("Start Warehouse Scene"))
    {
        m_startPressed = true;
    }
    ImGui::End();
}