#ifndef __SCENEWAREHOUSE_H__
#define __SCENEWAREHOUSE_H__

#include "scene.h"
#include "player.h"
#include "machine.h"
#include "moneybag.h"
#include "animatedsprite.h"
#include "particle.h"
#include "particlesystem.h"
#include "soundsystem.h"
#include "assistant.h"


#include <vector>
#include <map>
#include <random>
#include <memory>
#include <list>
#include <string>

struct TimedText {
	std::string text;
	int x, y;
	float timeRemaining;
	bool typeWriter;
	float typeTimer = 0.0f;
	size_t charsVisible = 0;
	std::string id;
};

class SceneWarehouse : public Scene
{
public:
	SceneWarehouse();
	~SceneWarehouse();

	bool Initialise(Renderer& renderer) override;
	void Process(float deltaTime, InputSystem& inputSystem) override;
	void Draw(Renderer& renderer) override;
	void DebugDraw() override;

	void InitCharSprites(Renderer& renderer);
	void DrawNumber(Renderer& renderer, int number, int startX, int startY);
	void DrawText(const std::string& text, int startX, int startY, float duration, bool typeWriter, const std::string& id = "");
	void RemoveTextById(const std::string& id);
	void DeleteTexts();

	bool StartProduction();
	void Production(float time);
	void Tutorial(Renderer& renderer);
	bool InitMachines(Renderer& renderer);
	void DisplayUpgrade(int mindex);
	void PauseMenu(InputSystem& input);

	float GetGrowInterval() const;
	void OnEnter() override;
	void OnExit() override;
	std::string FormatWithCommas(int value);
	void SkipToProduction();
	void MaxMachines();



private:

	SoundSystem m_soundSystem;
	float m_screenWidth;
	float m_screenHeight;
	Player* m_pPlayer;
	std::vector<Machine*> m_machines;
	Renderer* m_pRenderer;
	Sprite* m_pWarehouseBackground;
	Sprite* m_pBagSprite = nullptr;
	std::map<char, Sprite*> m_charSprites;
	std::vector<TimedText> m_activeTexts;
	int m_totalUpgradeLevel;
	float m_timer;
	float m_baseValue;
	int m_bevValue;

	float m_moneyGrowTimer;
	float m_growInterval;
	float m_productionSpeedMultiplier;

	//grow and spawn money bags seperately
	float m_moneySpawnTimer = 0.0f;
	float m_spawnInterval = 3.4f;

	int m_totalSold;

	int m_moneyPot;

	float m_tutInterval;
	int m_tutStage = 0;

	bool m_coinsAdded = false;
	bool m_displayingUpgrade = false;

	bool m_paused = false;
	bool m_skipped = false;
	std::vector<MoneyBag*> m_moneyBags;

	std::default_random_engine m_rng;
	std::uniform_real_distribution<float> m_spawnXDist;
	std::uniform_real_distribution<float> m_spawnYDist;

	std::vector<Particle> m_coinParticles;
	std::vector<Particle> m_sparkParticles; //for future
	Sprite* m_pCoinSprite = nullptr;
	Sprite* m_pSparkSprite = nullptr;

	std::list<ParticleSystem> m_particleSystems;

	Sprite* m_pPauseMenu;
	Sprite* m_pWinScreen;
	bool m_pauseKey = false;



	float m_lastMoneyCheckTime = 0.0f;
	int m_lastSoldAmount = 0;
	float m_moneyPerSecond = 0.0f;
	Assistant* m_pAssistant = nullptr;
	bool m_assistantUnlocked = false;
	bool m_won = false;
};

#endif // __SCENEWAREHOUSE_H__