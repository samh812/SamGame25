#include "splashscreens.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include "game.h"
#include "renderer.h"
#include "texture.h"
#include "sprite.h"


Splashscreens::Splashscreens()
{
}

Splashscreens::~Splashscreens()
{
    if (FMOD_splash) {
        delete FMOD_splash;
        FMOD_splash = nullptr;
    }

}

bool Splashscreens::Initialise(Renderer& renderer)
{
    m_timer = 0.0f;
    FMOD_splash = renderer.CreateSprite("../assets/fmod_white_logo.png");
    FMOD_splash->SetX(renderer.GetWidth() / 2);
    FMOD_splash->SetY(renderer.GetHeight() / 2);
    FMOD_splash->SetScale(0.6f);

    AUT_splash = renderer.CreateSprite("../assets/AUT_logo.png");
    AUT_splash->SetX(renderer.GetWidth() / 2);
    AUT_splash->SetY(renderer.GetHeight() / 2);
    AUT_splash->SetScale(0.5f);
    return true;
}

void Splashscreens::Process(float deltaTime, InputSystem& inputSystem)
{
    m_timer += deltaTime;
    FMOD_splash->Process(deltaTime);
    AUT_splash->Process(deltaTime);

    if (m_timer >= 5.0f) {
        Game::GetInstance().SetCurrentScene(1); //switch to SceneWarehouse
    }
}

void Splashscreens::Draw(Renderer& renderer)
{
    if (m_timer >= 0.0f && m_timer < 3.0f) {
        AUT_splash->Draw(renderer);
    }
    else if (m_timer >= 3.0f && m_timer <= 5.0f) {
        FMOD_splash->Draw(renderer);
    }

}

void Splashscreens::DebugDraw()
{
}