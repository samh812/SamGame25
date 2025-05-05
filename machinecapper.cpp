#include "machinecapper.h"
#include "renderer.h"

MachineCapper::MachineCapper() {}

MachineCapper::~MachineCapper() {}

bool MachineCapper::Initialise(Renderer& renderer)
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