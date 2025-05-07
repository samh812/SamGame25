#ifndef __SCENEWAREHOUSE_H__
#define __SCENEWAREHOUSE_H__

#include "scene.h"
#include "player.h"
#include "machine.h"
#include "moneybag.h"
#include <vector>
#include <map>
#include <random>


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
	std::vector<Machine*> m_machines;
	Renderer* m_pRenderer;
	Sprite* m_pWarehouseBackground;
	std::map<char, Sprite*> m_digitSprites;
	int m_totalUpgradeLevel;
	float m_timer;
	float m_baseValue;
	int m_bevValue;
	float m_interval;

	std::vector<MoneyBag*> m_moneyBags;
	float m_moneySpawnTimer = 0.0f;
	std::default_random_engine m_rng;
	std::uniform_real_distribution<float> m_spawnXDist;
	std::uniform_real_distribution<float> m_spawnYDist;
};

#endif // __SCENEWAREHOUSE_H__