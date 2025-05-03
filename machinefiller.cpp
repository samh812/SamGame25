#include "machinefiller.h"
#include "renderer.h"

MachineFiller::MachineFiller() {}

MachineFiller::~MachineFiller() {}

bool MachineFiller::Initialise(Renderer& renderer)
{
    if (!Machine::Initialise(renderer))
        return false;

    // Load the sprite specific to MachineFiller
    m_pSprite = renderer.CreateSprite("../assets/machine_level1.png");  // Your sprite path
    if (m_pSprite == nullptr)
    {
        return false;
    }

    return true;
}