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
#include "game.h"
#include "scene.h"
#include "xboxcontroller.h"
#include <iostream>
#include <string>
#include <set>
#include <chrono>

#include <fmod.hpp>
#include <fmod_errors.h>

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


    for (Machine* machine : m_machines)
    {
        delete machine;
    }
    m_machines.clear();



    delete m_pWarehouseBackground;
    m_pWarehouseBackground = nullptr;

    delete m_pPauseMenu;
    m_pPauseMenu = nullptr;

    delete m_pCoinSprite;
    m_pCoinSprite = nullptr;
    delete m_pBagSprite;
    m_pBagSprite = nullptr;
    for (auto& pair : m_charSprites)
    {
        delete pair.second;
    }
    m_charSprites.clear();

    for (MoneyBag* pBag : m_moneyBags)
    {
        delete pBag;  // Only deletes the MoneyBag, not its sprite
    }
    m_moneyBags.clear();
    m_soundSystem.Release();
}

void SceneWarehouse::OnEnter() {
    m_soundSystem.PlaySound("bgm");
    m_paused = false;


};

void SceneWarehouse::OnExit()
{
    m_soundSystem.StopSound("bgm");

    m_tutInterval = 2.0f;
    m_tutStage = 0;
    m_totalSold = 0;
    m_pPlayer->SpendMoney(m_pPlayer->GetMoney());
    m_totalUpgradeLevel = 0;

    for (Machine* machine : m_machines)
    {
        machine->SetUpgradeLevel(-1);
        machine->Upgrade();
    }
    for (MoneyBag* pBag : m_moneyBags)
    {
        pBag->Deactivate();
    }

    




}

bool SceneWarehouse::Initialise(Renderer& renderer)
{

    m_paused = false;
    m_screenWidth = static_cast<float>(renderer.GetWidth());
    m_screenHeight = static_cast<float>(renderer.GetHeight());

    if (!m_soundSystem.Initialise()) {
        std::cerr << "Failed to initialise FMOD system!" << std::endl;
        return false;
    }
    m_soundSystem.LoadSound("bgm", "../assets/sounds/bgm.mp3", true);

    m_soundSystem.LoadSound("coin", "../assets/sounds/coin.wav");


	m_tutInterval = 2.0f;
    m_pBagSprite = renderer.CreateSprite("../assets/moneyBag.png");
    m_pBagSprite->SetScale(0.13f);
    for (int i = 0; i < 10; ++i)
    {
        MoneyBag* pBag = new MoneyBag();
        pBag->Initialise(renderer);
        pBag->SetSprite(m_pBagSprite);  // all bags share the same sprite

        pBag->Deactivate();
        m_moneyBags.push_back(pBag);
    }
    DrawText("Shekels        $", m_screenWidth * 0.04f, m_screenHeight * 0.09f, 0.0f, true);
    DrawText("Beverages", m_screenWidth * 0.04f, m_screenHeight * 0.16f, 0.0f, true);

    m_pWarehouseBackground = renderer.CreateSprite("../assets/warehouse_background.png");

    m_pPauseMenu = renderer.CreateSprite("../assets/pauseMenu.png");
    float scaleX = static_cast<float>(renderer.GetWidth()) / m_pPauseMenu->GetWidth();
    float scaleY = static_cast<float>(renderer.GetHeight()) / m_pPauseMenu->GetHeight();
    float scale = std::max(scaleX, scaleY);  //ensuring background covers whole screen

    m_pPauseMenu->SetX(renderer.GetWidth() / 2);
    m_pPauseMenu->SetY(renderer.GetHeight() / 2);
    m_pPauseMenu->SetScale(scale);



    m_pCoinSprite = renderer.CreateSprite("../assets/coin.png");
    m_pCoinSprite->SetScale(0.5f);



    scaleX = static_cast<float>(renderer.GetWidth()) / m_pWarehouseBackground->GetWidth();
    scaleY = static_cast<float>(renderer.GetHeight()) / m_pWarehouseBackground->GetHeight();
    scale = std::max(scaleX, scaleY);  //ensuring background covers whole screen

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

    InitMachines(renderer);


	InitCharSprites(renderer);

    m_spawnXDist = std::uniform_real_distribution<float>(m_screenWidth * 0.1f, m_screenWidth * 0.9f);
    m_spawnYDist = std::uniform_real_distribution<float>(m_screenHeight*0.1f, m_screenHeight * 0.65f);




    return true;
}

void SceneWarehouse::Process(float deltaTime, InputSystem& inputSystem)
{
    XboxController* controller = inputSystem.GetController(0);


    //pause logic
    bool controllerPausePressed = controller != nullptr && controller->GetButtonState(SDL_CONTROLLER_BUTTON_B) == BS_PRESSED;
    bool keyboardPausePressed = inputSystem.GetKeyState(SDL_SCANCODE_ESCAPE) == BS_PRESSED;
    bool pausePressed = keyboardPausePressed || controllerPausePressed;

    if (pausePressed && !m_pauseKey)
    {
        m_paused = !m_paused;
    }
    m_pauseKey = pausePressed;


    if (!m_paused) {


        m_tutInterval += deltaTime;
        //m_pTitleText->Process(deltaTime);
        if (m_pPlayer)
        {
            m_pPlayer->Process(deltaTime, inputSystem);


            //check for upgrade collision
            for (size_t i = 0; i < m_machines.size(); ++i)
            {
                // if pmachine isinupgradearea, display upgrade cost and details, with smaller text? Or i can jsut display in a central place?
                if (m_machines[i]->IsPlayerInUpgradeArea(m_pPlayer)) //Upgrade on E or ENTER
                {

                    DisplayUpgrade(i);

                    bool controllerCheck = false;
                    if (controller != nullptr) {
                        controllerCheck = controller->GetButtonState(SDL_CONTROLLER_BUTTON_A) == BS_PRESSED;
                    }

                    if (m_machines[i]->GetUpgradeLevel() < m_machines[i]->GetNumUpgrades() && inputSystem.GetKeyState(SDL_SCANCODE_E) == BS_PRESSED || controllerCheck) {
                        int upgradeCost = m_machines[i]->GetUpgradeCost(); //get specific machine's upgrade cost
                        if (m_pPlayer->SpendMoney(upgradeCost)) { //if player has enough money
                            m_machines[i]->Upgrade();
                            m_totalUpgradeLevel++;

                        }
                    }

                }
                else {
                }

            }


            if (StartProduction()) //grow pot for every drink sold, but only spawn money bags occasionally
            {
                Production(deltaTime);
                while (m_moneyGrowTimer >= m_growInterval)
                {
                    m_moneyGrowTimer -= m_growInterval;
                    m_totalSold++;
                    m_moneyPot += m_bevValue;

                    if (m_moneySpawnTimer >= m_spawnInterval)
                    {
                        for (MoneyBag* pBag : m_moneyBags)
                        {
                            if (!pBag->IsActive())
                            {
                                Vector2 randPos(m_spawnXDist(m_rng), m_spawnYDist(m_rng));
                                pBag->SetValue(m_moneyPot);
                                pBag->Activate(randPos);
                                m_moneyPot = 0;
                                break;
                            }
                        }
                        m_moneySpawnTimer = 0.0f;
                    }
                }
            }

            //check player pickup

            for (MoneyBag* pBag : m_moneyBags)
            {

                bool controllerCheck = false;
                if (controller != nullptr) {
                    controllerCheck = controller->GetButtonState(SDL_CONTROLLER_BUTTON_A) == BS_PRESSED;
                }
                if (pBag->IsActive() && m_pPlayer->IsCollidingWith(*pBag)) {
                    if (inputSystem.GetKeyState(SDL_SCANCODE_E) == BS_PRESSED ||
                        controllerCheck) {
                        m_pPlayer->AddMoney(pBag->GetValue());

                        m_soundSystem.PlaySound("coin");
                        //trigger coin particle effect
                        ParticleSystem ps;
                        ps.Initialise(m_pCoinSprite, m_pPlayer, 50);
                        ps.ActivateAt(pBag->GetPosition());
                        m_particleSystems.push_back(std::move(ps));

                        pBag->Deactivate();
                    }
                }
            }
        }

        for (auto it = m_particleSystems.begin(); it != m_particleSystems.end(); ) {
            it->Update(deltaTime);
            if (it->IsFinished()) {
                it = m_particleSystems.erase(it);
            }
            else {
                ++it;
            }
        }

        for (Machine* pMachine : m_machines)
        {
            pMachine->Process(deltaTime, inputSystem);
        }



        for (Particle& particle : m_coinParticles) {
            particle.Update(deltaTime);
        }

        for (auto it = m_activeTexts.begin(); it != m_activeTexts.end(); ) {
            if (it->timeRemaining > 0.0f) {
                it->timeRemaining -= deltaTime;
                if (it->timeRemaining <= 0.0f) {
                    it = m_activeTexts.erase(it);
                    continue;
                }
            }

            if (it->typeWriter && it->charsVisible < it->text.size()) {
                it->typeTimer += deltaTime;
                if (it->typeTimer >= 0.05f) {
                    ++it->charsVisible;
                    it->typeTimer = 0.0f;
                }
            }

            ++it;
        }

        m_soundSystem.Update();
    }
    else {
        PauseMenu(inputSystem);
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


    for (MoneyBag* pBag : m_moneyBags)
    {

        pBag->Draw(renderer);

    }

    for (auto& ps : m_particleSystems) {
        ps.Draw(renderer);
    }
    if (m_pPlayer) {
        DrawNumber(renderer, m_pPlayer->GetMoney(), m_screenWidth * 0.185f, m_screenHeight*0.09f);
        DrawNumber(renderer, m_totalSold, m_screenWidth * 0.185f, m_screenHeight*0.16f);
    }


    for (const TimedText& timed : m_activeTexts) {
        int spacing = 14;
        int maxChars = std::min(timed.charsVisible, timed.text.length());

        for (int i = 0; i < maxChars; ++i) {
            char c = timed.text[i];
            auto it = m_charSprites.find(c);
            if (it != m_charSprites.end()) {
                Sprite* sprite = it->second;
                sprite->SetX(timed.x + (i * spacing));
                sprite->SetY(timed.y);
                sprite->Draw(renderer);
            }
        }
    }
    DeleteTexts();

    if (m_tutInterval >= 5.0f && m_tutStage < 7) {
        Tutorial(renderer);
        if (!m_coinsAdded) {
            ParticleSystem ps;
            ps.Initialise(m_pCoinSprite, m_pPlayer, 20);
            ps.ActivateAt(m_pPlayer->GetPosition());
            m_particleSystems.push_back(std::move(ps));
			m_coinsAdded = true;
        }


        m_tutInterval = 0.0f;
        m_tutStage++;



    }
    if (m_paused) {
        m_pPauseMenu->Draw(renderer);
    }
}
void SceneWarehouse::DebugDraw()
{
    if (m_pPlayer)
    {
  //      Vector2 playerPos = m_pPlayer->GetPosition();
  //      ImGui::Text("Player Position: X = %.2f, Y = %.2f", playerPos.x, playerPos.y);
  //      if (StartProduction()) {ImGui::Text("Production: ON Interval: %f", m_growInterval);}
  //      else{ImGui::Text("Production: OFF");}
  //      ImGui::Text("Beverage value (rounded int): %d base value: %f", m_bevValue, m_baseValue);
		ImGui::Text("Active texts count: %d", m_activeTexts.size());
        ImGui::Text("m_paused: %d", m_paused);
        // Show the frame rate (FPS)
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//ImGui::Text("Money Spawn interval: %f", m_spawnInterval);
		ImGui::Text("Money Grow interval: %f", m_growInterval);
		//ImGui::Text("Money Pot: %d", m_moneyPot);
  //      ImGui::Text("Tutorial interval: %f", m_tutInterval);



        //ACTUAL DEBUGGING

        //1. Grant player shekels
        static int giveMoney = 0;
        ImGui::InputInt("Give player shekels", &giveMoney);
        if (ImGui::Button("Apply")) {
            m_pPlayer->AddMoney(giveMoney);
        }
            // Money per second calculation
        float currentTime = ImGui::GetTime();
        if (currentTime - m_lastMoneyCheckTime >= 1.0f)
        {
            int currentMoney = m_totalSold; // Assumes GetMoney() exists
            int deltaMoney = currentMoney - m_lastMoneyAmount;
            m_moneyPerSecond = static_cast<float>(deltaMoney) / (currentTime - m_lastMoneyCheckTime);

            m_lastMoneyAmount = currentMoney;
            m_lastMoneyCheckTime = currentTime;
        }

        ImGui::Text("Bevs per second: %.6f", m_moneyPerSecond);
    }



    //void SceneBouncingBalls::DebugDraw
    //()
    //{
    //    ImGui::Text("Scene: Bouncing Balls");
    //    ImGui::SliderInt("Show Count", &m_iShowCount, 1, 10);
    //    static int editBallNumber = 0;
    //    ImGui::SliderInt("Edit ball", &editBallNumber, 0, 9);
    //    m_pBalls[editBallNumber]->DebugDraw();
    //}
    int i = 0;
    for (Machine* machine : m_machines)
    {
        if (machine)
        {
            bool inArea = machine->IsPlayerInUpgradeArea(m_pPlayer);
            bool upgraded = machine->IsUpgraded();
            int upgradeLevel = machine->GetUpgradeLevel(); // Get the upgrade level

            //ImGui::Text("Machine %d:", i+1);
            //Vector2 pos = machine->GetPosition();
            //ImGui::Text("machine position: X = %.2f, Y = %.2f", pos.x, pos.y);

            //ImGui::BulletText("In Upgrade Area: %s", inArea ? "YES" : "no");
            //ImGui::BulletText("Machine value increase float: %f", machine->GetValueIncreases());
            //ImGui::BulletText("Upgrade Level: %d", upgradeLevel); // Display upgrade level
            ++i;
        }
    }

    //for (MoneyBag* bag : m_moneyBags) {
    //    if (bag->IsActive()) {
    //        ImGui::Text("Bag %d:", i + 1);
    //        Vector2 pos = bag->GetPosition();
    //        ImGui::Text("Bag position: X = %.2f, Y = %.2f", pos.x, pos.y);


    //    }
    //}

}


void SceneWarehouse::InitCharSprites(Renderer& renderer) {
	int textSize = 30;
    for (char c = '0'; c <= '9'; ++c) {
        std::string text(1, c);
        renderer.CreateStaticText(text.c_str(), textSize);
        m_charSprites[c] = renderer.CreateSprite(text.c_str());
    }

    for (char c = 'A'; c <= 'Z'; ++c) {
        std::string text(1, c);
        renderer.CreateStaticText(text.c_str(), textSize);
        m_charSprites[c] = renderer.CreateSprite(text.c_str());
    }

    for (char c = 'a'; c <= 'z'; ++c) {
        std::string text(1, c);
        renderer.CreateStaticText(text.c_str(), textSize);
        m_charSprites[c] = renderer.CreateSprite(text.c_str());
    }
    const std::string extras = "!,:$.+%/";
    for (char c : extras) {
        std::string text(1, c);
        renderer.CreateStaticText(text.c_str(), textSize);
        m_charSprites[c] = renderer.CreateSprite(text.c_str());
    }
}

void SceneWarehouse::DrawNumber(Renderer& renderer, int number, int startX, int startY) {
    std::string numStr = FormatWithCommas(number);
    int spacing = 23;

    for (size_t i = 0; i < numStr.length(); ++i) {
        char c = numStr[i];

        if (m_charSprites.find(c) == m_charSprites.end()) {
            continue;
        }

        Sprite* sprite = m_charSprites[c];
        sprite->SetX(startX + (i * spacing));
        sprite->SetY(startY);
        sprite->Draw(renderer);
    }
}

std::string SceneWarehouse::FormatWithCommas(int value) {
    std::string numStr = std::to_string(value);
    int insertPosition = numStr.length() - 3;

    while (insertPosition > 0) {
        numStr.insert(insertPosition, ",");
        insertPosition -= 3;
    }

    return numStr;
}

void SceneWarehouse::DrawText(const std::string& text, int startX, int startY, float duration, bool typeWriter, const std::string& id) {
    TimedText timed;
    timed.text = text;
    timed.x = startX;
    timed.y = startY;
    timed.timeRemaining = duration;
    timed.typeWriter = typeWriter;
    timed.charsVisible = typeWriter ? 0 : text.length();
    timed.id = id;
    m_activeTexts.push_back(timed);
}

void SceneWarehouse::RemoveTextById(const std::string& id) {
    m_activeTexts.erase(
        std::remove_if(m_activeTexts.begin(), m_activeTexts.end(),
            [&](const TimedText& t) { return t.id == id; }),
        m_activeTexts.end());
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
	m_baseValue = 15.0f;
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

void SceneWarehouse::Tutorial(Renderer& renderer) {


    float xText = 0.60f;
    switch (m_tutStage) {
	    case 0:
		    DrawText("Hey, fresh meat!", m_screenWidth*xText, m_screenHeight*0.15f, 5.0f, true);
		    DrawText("This is your new home now", m_screenWidth * xText, m_screenHeight * 0.2f, 5.0f, true);
			m_coinsAdded = true;
		    break;
        case 1:
			m_coinsAdded = false;
            DrawText("Take these shekels", m_screenWidth * xText, m_screenHeight * 0.15f, 5.0f, true);
            m_pPlayer->AddMoney(40); //start with 40 shekels
            break;

        case 2:
            DrawText("Walk over and repair those machines!", m_screenWidth * xText, m_screenHeight * 0.15f, 5.0f, true);
            DrawText("We need to get production going FAST", m_screenWidth * xText, m_screenHeight * 0.2f, 5.0f, true);
            break;
		case 3:
            DrawText("Press E / A next to a machine to upgrade it", m_screenWidth * xText, m_screenHeight * 0.15f, 7.0f, true);
            DrawText("Fix all the machines to start production", m_screenWidth * xText, m_screenHeight * 0.12f, 7.0f, true);
            break;

        case 5:
            DrawText("Lets see...", m_screenWidth * xText, m_screenHeight * 0.15f, 5.0f, true);
            DrawText("One million beverages should do!", m_screenWidth * xText, m_screenHeight * 0.2f, 5.0f, true);
            break;
        case 6:
            DrawText("Press E / A to pick up money bags", m_screenWidth * xText, m_screenHeight * 0.15f, 7.0f, true);

            break;
        default:
	        break;
    }

}


bool SceneWarehouse::InitMachines(Renderer& renderer) {
    //placing machines based on screen resolution
    const int numMachines = 7;



    float yOffset = m_screenHeight * 0.85f; //dynamically placing machines near the bottom of the screen
    float totalWidth = 0.0f;
    std::vector<float> machineWidths;

    //determine the widths of all the machines so they can be dynamically placed
    for (int i = 0; i <= numMachines; ++i)
    {
        float width = (i == 1 || i == 3 || i == 5) ? 225.0f : 160.0f; //conveyors are 225 vs 160
        machineWidths.push_back(width);
        totalWidth += width;
    }

    float startX = m_screenWidth * 0.04f;
    float currentX = startX;

    //dynamic upgrade area size
    float dynamicYOffset = m_screenHeight * 0.2f;  //15% above the machine
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
            basePath = "../assets/machines/machine_bottler_";
            pMachine->SetUpgradeCosts({ 10, 75 , 1000, 10000, 30000}); // to lvl 1, to lvl 2
            pMachine->SetValueIncrease({ 1.0f, 1.0f, 1.5f , 2.3f, 2.7f, 3.0f }); // broken, lvl1, lvl2
        }
        else if (dynamic_cast<MachineConveyor*>(pMachine))
        {
            basePath = "../assets/machines/machine_conveyor_";
            pMachine->SetUpgradeCosts({ 5, 40, 1000, 10000, 30000});
            pMachine->SetValueIncrease({ 1.0f, 1.0f, 0.5f, 0.25f, 0.05f, 0.003f }); //0.8, 0.6 originally


        }
        else if (dynamic_cast<MachineFiller*>(pMachine))
        {
            basePath = "../assets/machines/machine_filler_";
            pMachine->SetUpgradeCosts({ 7, 80, 1000, 10000, 30000});
            pMachine->SetValueIncrease({ 1.0f, 1.0f, 1.6f, 2.3f, 2.7f, 3.0f });

        }
        else if (dynamic_cast<MachineCapper*>(pMachine))
        {
            basePath = "../assets/machines/machine_capper_";
            pMachine->SetUpgradeCosts({ 3, 60, 1000, 10000, 30000});
            pMachine->SetValueIncrease({ 1.0f, 1.0f, 1.4f, 2.3f, 2.7f, 3.0f });

        }
        else if (dynamic_cast<MachineLabeler*>(pMachine))
        {
            basePath = "../assets/machines/machine_labeler_";
            pMachine->SetUpgradeCosts({ 5, 65, 1000, 10000, 30000});
            pMachine->SetValueIncrease({ 1.0f, 1.0f, 1.7f, 2.3f, 2.7f, 3.0f });

        }
        for (int level = 0; level <= numUpgrades; ++level)
        {
            std::string fullPath = basePath + std::to_string(level) + ".png";

            //animated conveyor
            if (dynamic_cast<MachineConveyor*>(pMachine)) {
                std::unique_ptr<AnimatedSprite> animatedUpgradeSprite = std::unique_ptr<AnimatedSprite>(renderer.CreateAnimatedSprite(fullPath.c_str()));
                animatedUpgradeSprite->SetupFrames(225, 160);
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
        pMachine->SetUpgradeArea(Vector2(currentX, upgradeAreaYOffset), width, m_screenHeight * 0.125f); //using machine width

        m_machines.push_back(pMachine);
        currentX += width;
    }
    return true;
}


void SceneWarehouse::DisplayUpgrade(int mindex) {

    std::string currentLevel = std::to_string(m_machines[mindex]->GetUpgradeLevel());

    std::string nextCost = std::to_string(m_machines[mindex]->GetUpgradeCost());
    std::string upgradeDetails;
    int increase = (static_cast<int>(m_machines[mindex]->GetNextValueIncrease() * 100.0f));








    if (dynamic_cast<MachineConveyor*>(m_machines[mindex])) { //if conveyor display production increase
        upgradeDetails = "Production speed +";

        upgradeDetails += std::to_string(100-increase);
        upgradeDetails += "%";
    }
    else {                                                     //else display value increase
        upgradeDetails = "Beverage value +";
        upgradeDetails += std::to_string(increase);
        upgradeDetails += "%";

    }


    if (currentLevel == "0") { //if broken
        currentLevel = "Broken";
        upgradeDetails = " ";
        DrawText(currentLevel, m_screenWidth * 0.04f, m_screenHeight * 0.4f, 0.0f, false, "current_level");
    }
    else { //if not broken, show level 

        DrawText("Level ", m_screenWidth * 0.04f, m_screenHeight * 0.4f, 0.0f, false, "level");
        DrawText(currentLevel, m_screenWidth * 0.09f, m_screenHeight * 0.4f, 0.0f, false, "current_level");
    }


    if (nextCost == "0") { //if max upgrade level
        DrawText("Maxed out!", m_screenWidth * 0.04f, m_screenHeight * 0.45f, 0.0f, false, "upgrade_label");

    }
    else { //
        DrawText("Upgrade Cost $", m_screenWidth * 0.04f, m_screenHeight * 0.45f, 0.0f, false, "upgrade_label");
        DrawText(nextCost, m_screenWidth * 0.17f, m_screenHeight * 0.45f, 0.0f, false, "upgrade_cost");
        DrawText(upgradeDetails, m_screenWidth * 0.04f, m_screenHeight * 0.5f, 0.0f, false, "upgrade_details");

    }

}
void SceneWarehouse::DeleteTexts() {
    RemoveTextById("current_level");
    RemoveTextById("level");
    RemoveTextById("upgrade_cost");
    RemoveTextById("upgrade_label");
    RemoveTextById("upgrade_details");

}


void SceneWarehouse::PauseMenu(InputSystem& input) {


    XboxController* controller = input.GetController(0);


    bool controllerY = false;
    if (controller != nullptr) {
        controllerY = controller->GetButtonState(SDL_CONTROLLER_BUTTON_Y) == BS_PRESSED;
    }
    bool controllerX = false;
    if (controller != nullptr) {
        controllerX = controller->GetButtonState(SDL_CONTROLLER_BUTTON_X) == BS_PRESSED;
    }



    //Unpause/continue
    if (input.GetKeyState(SDL_SCANCODE_Y) == BS_PRESSED || controllerY)
    {
        //restart funciton here. Also restart on enter
        OnExit();
        OnEnter();

    }
    //Unpause/continue
    if (input.GetKeyState(SDL_SCANCODE_Q) == BS_PRESSED || controllerX)
    {
        Game::GetInstance().SetCurrentScene(1);

    }



}
