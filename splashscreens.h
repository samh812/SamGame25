#ifndef SPLASHSCREENS_H
#define SPLACHSCREENS_H

#include "scene.h"
#include "sprite.h"

class Splashscreens : public Scene
{
public:
    Splashscreens();
    virtual ~Splashscreens();

    bool Initialise(Renderer& renderer) override;
    void Process(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;
    void DebugDraw() override;

private:
    float m_timer;
    Sprite* FMOD_splash;
    Sprite* AUT_splash;
};

#endif 