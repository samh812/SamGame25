#include "scenewarehouse.h"
#include "renderer.h"

SceneWarehouse::SceneWarehouse()
    : m_pPlayer(nullptr)
{
}

SceneWarehouse::~SceneWarehouse()
{
    delete m_pPlayer;
}

bool SceneWarehouse::Initialise(Renderer& renderer)
{
    m_pPlayer = new Player();
    return m_pPlayer->Initialise(renderer);
}

void SceneWarehouse::Process(float deltaTime, InputSystem& inputSystem)
{
    if (m_pPlayer)
        m_pPlayer->Process(deltaTime, inputSystem);
}

void SceneWarehouse::Draw(Renderer& renderer)
{
    if (m_pPlayer)
        m_pPlayer->Draw(renderer);
}

void SceneWarehouse::DebugDraw()
{
    // Optional ImGui debug stuff for this scene
}