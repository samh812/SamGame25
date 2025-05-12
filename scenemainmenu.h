#ifndef SCENEMAINMENU_H
#define SCENEMAINMENU_H

#include "scene.h"
#include "sprite.h"

class SceneMainMenu : public Scene
{
public:
    SceneMainMenu();
    virtual ~SceneMainMenu();

    bool Initialise(Renderer& renderer) override;
    void Process(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;
    void DebugDraw() override;

    void OnEnter() override;
    void OnExit() override;

private:
    bool m_menuActive;
    Sprite* m_pMainMenuSprite;
};

#endif 