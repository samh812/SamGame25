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
	m_pPlayer = nullptr;

    for (Machine* machine : m_machines)
    {
        delete machine;
    }
    m_machines.clear();

    delete m_pWarehouseBackground;
    m_pWarehouseBackground = nullptr;
}

bool SceneWarehouse::Initialise(Renderer& renderer)
{


    m_pWarehouseBackground = renderer.CreateSprite("../assets/warehouse_background.png");


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

    //placing machines based on screen resolution
    const int numMachines = 5;
    float screenWidth = static_cast<float>(renderer.GetWidth());
    float screenHeight = static_cast<float>(renderer.GetHeight());

    float yOffset = screenHeight * 0.85f; //dynamically placing machines near the bottom of the screen
    float totalWidth = 0.0f;
    std::vector<float> machineWidths;

    //determine the widths of all the machines so they can be dynamically placed
    for (int i = 0; i < numMachines; ++i)
    {
        float width = (i == 1 || i == 3) ? 225.0f : 160.0f; //conveyors are 225 vs 160
        machineWidths.push_back(width);
        totalWidth += width;
    }

    //calculate startX to place the machines starting from the left corner
    float startX = screenWidth * 0.03f;
    float currentX = startX;

    for (int i = 0; i < numMachines; ++i)
    {
        Machine* pMachine = nullptr;

        switch (i)
        {
        case 0: case 4:
            pMachine = new MachineBottler();
            break;
        case 1: case 3:
            pMachine = new MachineConveyor();
            break;
        case 2:
            pMachine = new MachineFiller();
            break;
        }

        if (!pMachine->Initialise(renderer))
        {
            delete pMachine;
            return false;
        }

        float width = machineWidths[i];
        pMachine->SetPosition(Vector2(currentX + width / 2.0f, yOffset));
        m_machines.push_back(pMachine);

        currentX += width;
    }

    return true;
}

void SceneWarehouse::Process(float deltaTime, InputSystem& inputSystem)
{
    if (m_pPlayer)
        m_pPlayer->Process(deltaTime, inputSystem);

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
    //do later
}