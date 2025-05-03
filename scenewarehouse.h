#ifndef __SCENEWAREHOUSE_H__
#define __SCENEWAREHOUSE_H__

#include "scene.h"
#include "player.h"
#include "machine.h"
#include <vector>


class SceneWarehouse : public Scene
{
public:
	SceneWarehouse();
	~SceneWarehouse();

	bool Initialise(Renderer& renderer) override;
	void Process(float deltaTime, InputSystem& inputSystem) override;
	void Draw(Renderer& renderer) override;
	void DebugDraw() override;

private:
	Player* m_pPlayer;
	std::vector<Machine*> m_machines;
};

#endif // __SCENEWAREHOUSE_H__