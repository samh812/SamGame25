#ifndef __SCENEWAREHOUSE_H__
#define __SCENEWAREHOUSE_H__

#include "scene.h"
#include "player.h"
#include "machine.h"
#include "moneybag.h"
#include "animatedsprite.h"
#include <vector>
#include <map>
#include <random>
#include <memory>



class SceneWarehouse : public Scene
{
public:
	SceneWarehouse();
	~SceneWarehouse();

	bool Initialise(Renderer& renderer) override;
	void Process(float deltaTime, InputSystem& inputSystem) override;
	void Draw(Renderer& renderer) override;
	void DebugDraw() override;

	void InitDigitSprites(Renderer& renderer);
	void DrawNumber(Renderer& renderer, int number, int startX, int startY);

	bool StartProduction();
	void Production(float time);


private:
	Player* m_pPlayer;
	AnimatedSprite* m_pPlayerSprite;
	std::vector<Machine*> m_machines;
	Renderer* m_pRenderer;
	Sprite* m_pWarehouseBackground;
	Sprite* m_pBagSprite = nullptr;
	std::map<char, Sprite*> m_digitSprites;
	int m_totalUpgradeLevel;
	float m_timer;
	float m_baseValue;
	int m_bevValue;

	float m_moneyGrowTimer;
	float m_growInterval;

	//grow and spawn money bags seperately
	float m_moneySpawnTimer = 0.0f;
	float m_spawnInterval = 3.8f;

	int m_totalSold;

	int m_moneyPot;
	std::vector<MoneyBag*> m_moneyBags;

	std::default_random_engine m_rng;
	std::uniform_real_distribution<float> m_spawnXDist;
	std::uniform_real_distribution<float> m_spawnYDist;
};

#endif // __SCENEWAREHOUSE_H__