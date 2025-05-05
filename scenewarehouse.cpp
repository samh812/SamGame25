#include "scenewarehouse.h"
#include "renderer.h"
#include "machine.h"
#include "machinebottler.h"
#include "machinefiller.h"
#include "machineconveyor.h"
#include "texture.h"
#include "sprite.h"
#include "player.h"
#include <iostream>
#include <string>

#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"

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
    float scale = std::max(scaleX, scaleY);  //ensuring background covers whole screen

    m_pWarehouseBackground->SetX(renderer.GetWidth() / 2);
    m_pWarehouseBackground->SetY(renderer.GetHeight() / 2);
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

    float startX = screenWidth * 0.03f;
    float currentX = startX;

    //dynamic upgrade area size
    float dynamicYOffset = screenHeight * 0.15f;  //15% above the machine
    float upgradeAreaYOffset = yOffset - dynamicYOffset;

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
        std::string basePath;
        int numUpgrades = 2;

        if (dynamic_cast<MachineBottler*>(pMachine))
        {
            basePath = "../assets/machine_bottler_";
        }
        else if (dynamic_cast<MachineConveyor*>(pMachine))
        {
            basePath = "../assets/machine_conveyor_";
        }
        else if (dynamic_cast<MachineFiller*>(pMachine))
        {
            basePath = "../assets/machine_bottler_";/*"../assets/machine_filler_";*/
        }

        for (int level = 0; level <= numUpgrades; ++level)
        {
            std::string fullPath = basePath + std::to_string(level) + ".png";
            Sprite* upgradeSprite = renderer.CreateSprite(fullPath.c_str());
            if (upgradeSprite)
            {
                pMachine->AddUpgradeSprite(upgradeSprite);
            }
        }

        if (!pMachine->GetUpgradeSprites().empty())
        {
            pMachine->SetSprite(pMachine->GetUpgradeSprites()[0]);  //base sprite
        }
        float width = machineWidths[i];
        pMachine->SetPosition(Vector2(currentX + width / 2.0f, yOffset));

        //set upgrade area width to match the specific machine width
        pMachine->SetUpgradeArea(Vector2(currentX, upgradeAreaYOffset), width, screenHeight * 0.05f); //using machine width

        m_machines.push_back(pMachine);
        currentX += width;
    }

    return true;
}

void SceneWarehouse::Process(float deltaTime, InputSystem& inputSystem)
{
    if (m_pPlayer)
    {
        m_pPlayer->Process(deltaTime, inputSystem);

        //check for upgrade collision
        for (Machine* pMachine : m_machines)
        {

            if (pMachine->IsPlayerInUpgradeArea(m_pPlayer) && inputSystem.GetKeyState(SDL_SCANCODE_E) == BS_PRESSED) //Upgrade on E or ENTER
            {
                pMachine->Upgrade();
            }
        }
    }



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
    if (m_pPlayer)
    {
        Vector2 playerPos = m_pPlayer->GetPosition();
        ImGui::Text("Player Position: X = %.2f, Y = %.2f", playerPos.x, playerPos.y);
    }

    int i = 0;
    for (Machine* machine : m_machines)
    {
        if (machine)
        {
            bool inArea = machine->IsPlayerInUpgradeArea(m_pPlayer);
            bool upgraded = machine->IsUpgraded();
            int upgradeLevel = machine->GetUpgradeLevel(); // Get the upgrade level

            ImGui::Text("Machine %d:", i);
            ImGui::BulletText("In Upgrade Area: %s", inArea ? "YES" : "no");
            ImGui::BulletText("Upgraded: %s", upgraded ? "YES" : "no");
            ImGui::BulletText("Upgrade Level: %d", upgradeLevel); // Display upgrade level
            ++i;
        }
    }
}