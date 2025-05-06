#ifndef __SCENEWAREHOUSE_H__
#define __SCENEWAREHOUSE_H__

#include "scene.h"
#include "player.h"
#include "machine.h"
#include <vector>
#include <map>


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
};

#endif // __SCENEWAREHOUSE_H__