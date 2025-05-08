#include "scenewarehouse.h"
#include "renderer.h"
#include "machine.h"
#include "machinebottler.h"
#include "machinefiller.h"
#include "machineconveyor.h"
#include "machinecapper.h"
#include "machinelabeler.h"
#include "texture.h"
#include "sprite.h"
#include "player.h"
#include <iostream>
#include <string>
#include <set>
#include <chrono>

#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"

SceneWarehouse::SceneWarehouse()
    : m_rng(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()))

{
}

SceneWarehouse::~SceneWarehouse()
{
    delete m_pPlayer;
	m_pPlayer = nullptr;

	delete m_pPlayerSprite;
	m_pPlayerSprite = nullptr;
    for (Machine* machine : m_machines)
    {
        delete machine;
    }
    m_machines.clear();



    delete m_pWarehouseBackground;
    m_pWarehouseBackground = nullptr;

    delete m_pCoinSprite;
    m_pCoinSprite = nullptr;
    delete m_pBagSprite;
    m_pBagSprite = nullptr;
    for (auto& pair : m_digitSprites)
    {
        delete pair.second;
    }
    m_digitSprites.clear();

    for (MoneyBag* pBag : m_moneyBags)
    {
        printf("Deleting bag at address: %p\n", pBag);
        delete pBag;  // Only deletes the MoneyBag, not its sprite
    }
    m_moneyBags.clear();
}

bool SceneWarehouse::Initialise(Renderer& renderer)
{
    m_pWarehouseBackground = renderer.CreateSprite("../assets/warehouse_background.png");

    m_pPlayerSprite = renderer.CreateAnimatedSprite("../assets/ball.png");
    m_pPlayerSprite->SetupFrames(64, 64);
    m_pPlayerSprite->SetLooping(true);
    m_pPlayerSprite->SetFrameDuration(10.0f);
    m_pPlayerSprite->Animate();
    m_pPlayerSprite->SetScale(1.0f);

	m_pPlayerSprite->SetX(renderer.GetWidth() / 2);
    m_pPlayerSprite->SetY(renderer.GetHeight() / 2);

    m_pCoinSprite = renderer.CreateSprite("../assets/coin.png");

    // Create a pool of coin particles
    for (int i = 0; i < 50; ++i) {
        Particle particle;
        particle.Initialise(m_pCoinSprite);
        m_coinParticles.push_back(particle);
    }

    float scaleX = static_cast<float>(renderer.GetWidth()) / m_pWarehouseBackground->GetWidth();
    float scaleY = static_cast<float>(renderer.GetHeight()) / m_pWarehouseBackground->GetHeight();
    float scale = std::max(scaleX, scaleY);  //ensuring background covers whole screen

    m_pWarehouseBackground->SetX(renderer.GetWidth() / 2);
    m_pWarehouseBackground->SetY(renderer.GetHeight() / 2);
    m_pWarehouseBackground->SetScale(scale);

    m_pPlayer = new Player();
    if (!m_pPlayer->Initialise(renderer))
    {
        delete m_pPlayer;
        m_pPlayer = nullptr;
        return false;
    }
	m_pPlayer->AddMoney(400); //start with 40 shekels


    //placing machines based on screen resolution
    const int numMachines = 7;
    float screenWidth = static_cast<float>(renderer.GetWidth());
    float screenHeight = static_cast<float>(renderer.GetHeight());

    float yOffset = screenHeight * 0.85f; //dynamically placing machines near the bottom of the screen
    float totalWidth = 0.0f;
    std::vector<float> machineWidths;

    //determine the widths of all the machines so they can be dynamically placed
    for (int i = 0; i <= numMachines; ++i)
    {
        float width = (i == 1 || i == 3 || i == 5) ? 225.0f : 160.0f; //conveyors are 225 vs 160
        machineWidths.push_back(width);
        totalWidth += width;
    }

    float startX = screenWidth * 0.04f;
    float currentX = startX;

    //dynamic upgrade area size
    float dynamicYOffset = screenHeight * 0.15f;  //15% above the machine
    float upgradeAreaYOffset = yOffset - dynamicYOffset;

    for (int i = 0; i < numMachines; ++i)
    {
        Machine* pMachine = nullptr;

        switch (i)
        {
        case 0:
            pMachine = new MachineBottler();
            break;
        case 1: case 3: case 5:
            pMachine = new MachineConveyor();
            break;
        case 2:
            pMachine = new MachineFiller();
            break;
		case 4:
			pMachine = new MachineCapper();
			break;
		case 6:
			pMachine = new MachineLabeler();
			break;
		default:
			break;
        }

        if (!pMachine->Initialise(renderer))
        {
            delete pMachine;
            return false;
        }
        std::string basePath;
        int numUpgrades = pMachine->GetNumUpgrades();

        if (dynamic_cast<MachineBottler*>(pMachine))
        {
            basePath = "../assets/machine_bottler_";
            pMachine->SetUpgradeCosts({ 10, 75 }); // to lvl 1, to lvl 2
            pMachine->SetValueIncrease({ 1.0f, 1.9f, 2.3f }); // broken, lvl1, lvl2
        }
        else if (dynamic_cast<MachineConveyor*>(pMachine))
        {
            basePath = "../assets/machine_conveyor_";
            pMachine->SetUpgradeCosts({ 5, 40 });
            pMachine->SetValueIncrease({ 1.0f, 0.2f, 0.1f }); //0.8, 0.6 originally


        }
        else if (dynamic_cast<MachineFiller*>(pMachine))
        {
            basePath = "../assets/machine_filler_";
            pMachine->SetUpgradeCosts({ 7, 80 });
            pMachine->SetValueIncrease({ 1.0f, 1.6f, 1.9f });

        }
        else if (dynamic_cast<MachineCapper*>(pMachine))
        {
            basePath = "../assets/machine_capper_";
            pMachine->SetUpgradeCosts({ 3, 60 });
            pMachine->SetValueIncrease({ 1.0f, 1.5f, 1.8f });

        }
        else if (dynamic_cast<MachineLabeler*>(pMachine))
        {
            basePath = "../assets/machine_labeler_";
            pMachine->SetUpgradeCosts({ 5, 65 });
            pMachine->SetValueIncrease({ 1.0f, 1.5f, 1.9f });

        }
        for (int level = 0; level <= numUpgrades; ++level)
        {
            std::string fullPath = basePath + std::to_string(level) + ".png";

            //animated conveyor
            if (dynamic_cast<MachineConveyor*>(pMachine)) {
                std::unique_ptr<AnimatedSprite> animatedUpgradeSprite = std::unique_ptr<AnimatedSprite>(renderer.CreateAnimatedSprite(fullPath.c_str()));
                animatedUpgradeSprite->SetupFrames(128, 64);
                animatedUpgradeSprite->SetLooping(true);
                animatedUpgradeSprite->Animate();
                animatedUpgradeSprite->SetScale(1.0f);
                pMachine->AddAnimatedUpgradeSprite(std::move(animatedUpgradeSprite));
			}
            else //normal machine
            {
                std::unique_ptr<Sprite> upgradeSprite = std::unique_ptr<Sprite>(renderer.CreateSprite(fullPath.c_str()));
                pMachine->AddUpgradeSprite(std::move(upgradeSprite));
                //upgradeSprite->SetScale(0.25f); //other machines scale
            }

        }
        if (dynamic_cast<MachineConveyor*>(pMachine)) {
            if (!pMachine->GetAnimatedUpgradeSprites().empty())
            {

                pMachine->SetAnimatedSprite(pMachine->GetAnimatedUpgradeSprites()[0]);

            }
        }
        else {
            if (!pMachine->GetUpgradeSprites().empty())
            {
                pMachine->SetSprite(pMachine->GetUpgradeSprites()[0]);

            }
        }
        float width = machineWidths[i];
        pMachine->SetPosition(Vector2(currentX + width / 2.0f, yOffset));

        //set upgrade area width to match the specific machine width
        pMachine->SetUpgradeArea(Vector2(currentX, upgradeAreaYOffset), width, screenHeight * 0.075f); //using machine width

        m_machines.push_back(pMachine);
        currentX += width;
    }

	InitDigitSprites(renderer);

    m_pBagSprite = renderer.CreateSprite("../assets/ball.png");
    m_pBagSprite->SetScale(0.13f);

    m_spawnXDist = std::uniform_real_distribution<float>(screenWidth * 0.1f, screenWidth * 0.9f);
    m_spawnYDist = std::uniform_real_distribution<float>(screenHeight*0.1f, screenHeight * 0.65f);

    for (int i = 0; i < 10; ++i)
    {
        MoneyBag* pBag = new MoneyBag();
        pBag->Initialise(renderer);
        pBag->SetSprite(m_pBagSprite);  // all bags share the same sprite

        pBag->Deactivate();
        m_moneyBags.push_back(pBag);
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

            if (pMachine->GetUpgradeLevel() < pMachine ->GetNumUpgrades() && pMachine->IsPlayerInUpgradeArea(m_pPlayer) && inputSystem.GetKeyState(SDL_SCANCODE_E) == BS_PRESSED) //Upgrade on E or ENTER
            {
				int upgradeCost = pMachine->GetUpgradeCost(); //get specific machine's upgrade cost
                if (m_pPlayer->SpendMoney(upgradeCost)) { //if player has enough money
                    pMachine->Upgrade();
                    m_totalUpgradeLevel++;

                }
				else {
				}
            }

        }
		if (StartProduction()) {
            Production(deltaTime);
		}


        if (StartProduction() && m_moneyGrowTimer >= m_growInterval) //grow pot for every drink sold, but only spawn money bags occasionally
        {
			m_moneyPot += m_bevValue;//add money to a total pot every time beverage is sold
			m_totalSold++;
            if (m_moneySpawnTimer >= m_spawnInterval) {//if bag is ready to be spawned
                for (MoneyBag* pBag : m_moneyBags)
                {
                    if (!pBag->IsActive())
                    {
                        Vector2 randPos(m_spawnXDist(m_rng), m_spawnYDist(m_rng));
                        pBag->SetValue(m_moneyPot);
                        pBag->Activate(randPos);
						m_moneyPot = 0; //reset pot after spawning
                        break;
                    }
                }
				m_moneySpawnTimer = 0.0f;
            }
            m_moneyGrowTimer = 0.0f;
        }

        //check player pickup
        for (MoneyBag* pBag : m_moneyBags)
        {
            if (pBag->IsActive() && m_pPlayer->IsCollidingWith(*pBag)) {
                if (inputSystem.GetKeyState(SDL_SCANCODE_E) == BS_PRESSED) {
                    m_pPlayer->AddMoney(pBag->GetValue());

                    //trigger coin particle effect
                    for (Particle& particle : m_coinParticles) {
                        if (!particle.IsAlive()) {
                            particle.Activate(pBag->GetPosition());
                        }
                    }

                    pBag->Deactivate();
                }
            }
        }
    }



    for (Machine* pMachine : m_machines)
    {
        pMachine->Process(deltaTime, inputSystem);
    }

    for (auto& machine : m_machines)
    {
        if (machine)
        {
            machine->Process(deltaTime, inputSystem);
        }
    }

    m_pPlayerSprite->Process(deltaTime);
    for (Particle& particle : m_coinParticles) {
        particle.Update(deltaTime);
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

    if (m_pPlayer) {
        DrawNumber(renderer, m_pPlayer->GetMoney(), 100, 80);
        DrawNumber(renderer, m_totalSold, 100, 180);
    }
    for (MoneyBag* pBag : m_moneyBags)
    {

        pBag->Draw(renderer);

    }
	m_pPlayerSprite->Draw(renderer);
    for (Particle& particle : m_coinParticles) {
        particle.Draw(renderer);
    }
}
void SceneWarehouse::DebugDraw()
{
    if (m_pPlayer)
    {
        Vector2 playerPos = m_pPlayer->GetPosition();
        ImGui::Text("Player Position: X = %.2f, Y = %.2f", playerPos.x, playerPos.y);
        if (StartProduction()) {ImGui::Text("Production: ON Interval: %f", m_growInterval);}
        else{ImGui::Text("Production: OFF");}
        ImGui::Text("Beverage value (rounded int): %d base value: %f", m_bevValue, m_baseValue);
		ImGui::Text("Player Money: %d", m_pPlayer->GetMoney());

		ImGui::Text("Money Spawn interval: %f", m_spawnInterval);
		ImGui::Text("Money Grow interval: %f", m_growInterval);
		ImGui::Text("Money Pot: %d", m_moneyPot);
    }


    int i = 0;
    for (Machine* machine : m_machines)
    {
        if (machine)
        {
            bool inArea = machine->IsPlayerInUpgradeArea(m_pPlayer);
            bool upgraded = machine->IsUpgraded();
            int upgradeLevel = machine->GetUpgradeLevel(); // Get the upgrade level

            ImGui::Text("Machine %d:", i+1);
            Vector2 pos = machine->GetPosition();
            ImGui::Text("machine position: X = %.2f, Y = %.2f", pos.x, pos.y);

            ImGui::BulletText("In Upgrade Area: %s", inArea ? "YES" : "no");
            ImGui::BulletText("Machine value increase float: %f", machine->GetValueIncreases());
            ImGui::BulletText("Upgrade Level: %d", upgradeLevel); // Display upgrade level
            ++i;
        }
    }

    for (MoneyBag* bag : m_moneyBags) {
        if (bag->IsActive()) {
            ImGui::Text("Bag %d:", i + 1);
            Vector2 pos = bag->GetPosition();
            ImGui::Text("Bag position: X = %.2f, Y = %.2f", pos.x, pos.y);


        }
    }

}


void SceneWarehouse::InitDigitSprites(Renderer& renderer) {
    for (char digit = '0'; digit <= '9'; ++digit) {
		std::string text(1, digit);
		renderer.CreateStaticText(text.c_str(), 50);
		m_digitSprites[digit] = renderer.CreateSprite(text.c_str());
    }
}

void SceneWarehouse::DrawNumber(Renderer& renderer, int number, int startX, int startY) {

	std::string numStr = std::to_string(number);
    int spacing = 30;

    for (size_t i = 0; i < numStr.length(); ++i) {
        char digit = numStr[i];
        Sprite* sprite = m_digitSprites[digit];
        sprite->SetX(startX + (i * spacing));
        sprite->SetY(startY);
        sprite->Draw(renderer);
    }
}

bool SceneWarehouse::StartProduction() {
    if (m_totalUpgradeLevel >= 7) {
        return true;
    }
	else {
		return false;
	}
}

void SceneWarehouse::Production(float time) {
    m_moneyGrowTimer += time;
    m_moneySpawnTimer += time;
	m_baseValue = 1.0f;
    m_growInterval = 2.0f;
    for (Machine* machine : m_machines) {

        if (machine) {
            if (dynamic_cast<MachineConveyor*>(machine)) {
				m_growInterval *= machine->GetValueIncreases();


            }
            else {
                m_baseValue *= machine->GetValueIncreases();
            }
        }
        else {
            continue;
        }

    }
    m_bevValue = static_cast<int>(std::round(m_baseValue));

}

float SceneWarehouse::GetGrowInterval() const{
	return m_growInterval;
}


