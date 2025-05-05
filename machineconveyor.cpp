#include "machineconveyor.h"
#include "renderer.h"

MachineConveyor::MachineConveyor() {}

MachineConveyor::~MachineConveyor() {}

bool MachineConveyor::Initialise(Renderer& renderer)
{
    if (!Machine::Initialise(renderer))
        return false;


    m_pSprite = renderer.CreateSprite("../assets/explosion.png");
    if (m_pSprite == nullptr)
    {
        return false;
    }

    return true;
}