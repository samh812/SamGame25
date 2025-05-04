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

    // Create machines in pattern: Bottler > Conveyor > Filler > Conveyor > Bottler
    float currentX = 80.0f;
    float yOffset = 730.0f;

    for (int i = 0; i < 5; ++i)
    {
        Machine* pMachine = nullptr;

        switch (i)
        {
        case 0:
            pMachine = new MachineBottler();
            break;
        case 1:
            pMachine = new MachineConveyor();
            break;
        case 2:
            pMachine = new MachineFiller();
            break;
        case 3:
            pMachine = new MachineConveyor();
            break;
        case 4:
            pMachine = new MachineBottler();
            break;
        }

        if (!pMachine->Initialise(renderer))
        {
            delete pMachine;
            return false;
        }

        //determine width of machine
        float width = dynamic_cast<MachineConveyor*>(pMachine) ? 225.0f : 160.0f;


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