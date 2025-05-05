#include "machinebottler.h"
#include "renderer.h"


MachineBottler::MachineBottler() {}

MachineBottler::~MachineBottler() {}

bool MachineBottler::Initialise(Renderer& renderer)
{
    if (!Machine::Initialise(renderer))
        return false;


    m_pSprite = renderer.CreateSprite("../assets/explosion.png");

    //m_pSpriteBroken = renderer.CreateSprite("../assets/bottler_00.png");
    if (m_pSprite == nullptr)
    {
        return false;
    }

    return true;
}
