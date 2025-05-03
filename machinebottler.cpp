#include "machinebottler.h"
#include "renderer.h"

MachineBottler::MachineBottler(){}
MachineBottler::~MachineBottler() {}


bool MachineBottler::Initialise(Renderer& renderer)
{
	if (!Machine::Initialise(renderer))
	{
		return false;


		m_pSprite = m_pRenderer->CreateSprite("../assets/machine_level1.png"); 
	}
}