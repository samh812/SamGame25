#include "scenewarehouse.h"
#include "renderer.h"
#include "machine.h"
#include "machinebottler.h"
#include "machinefiller.h"
#include "machineconveyor.h"
#include "texture.h"
#include "sprite.h"

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


    m_pWarehouseBackground = renderer.CreateSprite("../assets/background.png");


    float scaleX = static_cast<float>(renderer.GetWidth()) / m_pWarehouseBackground->GetWidth();
    float scaleY = static_cast<float>(renderer.GetHeight()) / m_pWarehouseBackground->GetHeight();
    float scale = std::max(scaleX, scaleY);  // Use max to ensure it covers the whole screen

    m_pWarehouseBackground->SetX(renderer.GetWidth()/2);
	m_pWarehouseBackground->SetY(renderer.GetHeight()/2);
    m_pWarehouseBackground->SetScale(scale);


    m_pPlayer = new Player();
    if (!m_pPlayer->Initialise(renderer))
    {
        return false;
    }

    // Create machines with alternating types: MachineType1 > Conveyor > MachineType2 > Conveyor > MachineType1

    float startX = 100.0f;  // Starting X position
    float yOffset = 300.0f; // Fixed Y position

    for (int i = 0; i < 5; ++i)  // Create 5 machines for example
    {
        Machine* pMachine = nullptr;

        // Alternate between types of machines
        if (i % 2 == 0) {
            // Even index, use MachineType1
            pMachine = new MachineBottler();
        }
        else if (i % 2 == 1 && (i / 2) % 2 == 0) {
            // Odd index, use MachineTypeConveyor
            pMachine = new MachineConveyor();
        }
        else {
            // Use MachineType2 for the next odd index
            pMachine = new MachineFiller();
        }

        if (!pMachine->Initialise(renderer))
        {
            return false;  // If any machine fails to initialize, return false
        }

        // Set position based on index
        float xOffset = startX + (i * 150.0f);  // 150 units between each machine
        pMachine->SetPosition(Vector2(xOffset, yOffset));  // Set the machine's position
        m_machines.push_back(pMachine);  // Add the machine to the vector
    }

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
    if (m_pWarehouseBackground)
    {
        m_pWarehouseBackground->Draw(renderer);
    }

    if (m_pPlayer)
        m_pPlayer->Draw(renderer);

    // Draw all machines in the vector
    for (auto& machine : m_machines)
    {
        if (machine)
        {
            machine->Draw(renderer);
        }
    }
}

void SceneWarehouse::DebugDraw()
{
    // Optional ImGui debug stuff for this scene
}