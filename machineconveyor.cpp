#include "machineconveyor.h"
#include "renderer.h"

MachineConveyor::MachineConveyor() {}

MachineConveyor::~MachineConveyor() {}

bool MachineConveyor::Initialise(Renderer& renderer)
{
    if (!Machine::Initialise(renderer))
        return false;

    // Load the sprite specific to MachineTypeConveyor
    m_pSprite = renderer.CreateSprite("../assets/machine_level2.png");  // Your conveyor sprite path
    if (m_pSprite == nullptr)
    {
        return false;
    }

    return true;
}