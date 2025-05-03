#include "scenewarehouse.h"
#include "renderer.h"
#include "machine.h"

SceneWarehouse::SceneWarehouse()
    : m_pPlayer(nullptr)
{
}

SceneWarehouse::~SceneWarehouse()
{
    delete m_pPlayer;
    for (Machine* machine : m_machines)
    {
        delete machine;
    }
    m_machines.clear();
}

bool SceneWarehouse::Initialise(Renderer& renderer)
{
    m_pPlayer = new Player();
    if (!m_pPlayer->Initialise(renderer))
    {
        return false;
    }

    // Create multiple machines and give them different positions
    for (int i = 0; i < 4; ++i)  // Create 5 machines for example
    {
        Machine* pMachine = new Machine();
        if (!pMachine->Initialise(renderer))
        {
            return false;  // If any machine fails to initialize, return false
        }

        // Set different positions for each machine
        // Example: Distribute machines horizontally with a gap of 150 units
        float xOffset = 150.0f * i;  // 150 units between each machine
        float yOffset = 300.0f;      // Same y position for all machines (can change to make it more dynamic)

        pMachine->SetPosition(Vector2(xOffset, yOffset));  // Set the machine's position
        m_machines.push_back(pMachine);  // Add the machine to the vector
    }

    return true;

    return true;
}

void SceneWarehouse::Process(float deltaTime, InputSystem& inputSystem)
{
    if (m_pPlayer)
        m_pPlayer->Process(deltaTime, inputSystem);

    // Process all machines
    for (Machine* pMachine : m_machines)
    {
        pMachine->Process(deltaTime, inputSystem);
    }
}

void SceneWarehouse::Draw(Renderer& renderer)
{
    if (m_pPlayer)
        m_pPlayer->Draw(renderer);
    // Draw all machines
    for (Machine* pMachine : m_machines)
    {
        pMachine->Draw(renderer);
    }
}

void SceneWarehouse::DebugDraw()
{
    // Optional ImGui debug stuff for this scene
}