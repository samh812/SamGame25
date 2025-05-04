#include "machinefiller.h"
#include "renderer.h"

MachineFiller::MachineFiller() {}

MachineFiller::~MachineFiller() {}

bool MachineFiller::Initialise(Renderer& renderer)
{
    if (!Machine::Initialise(renderer))
        return false;


    m_pSprite = renderer.CreateSprite("../assets/filler_00.png");
    if (m_pSprite == nullptr)
    {
        return false;
    }

    return true;
}