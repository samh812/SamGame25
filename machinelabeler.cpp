#include "machinelabeler.h"
#include "renderer.h"

MachineLabeler::MachineLabeler() {}

MachineLabeler::~MachineLabeler() {
}

bool MachineLabeler::Initialise(Renderer& renderer)
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