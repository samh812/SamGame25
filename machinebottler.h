#ifndef MACHINEBOTTLER_H
#define MACHINEBOTTLER_H

#include "machine.h"

class MachineBottler : public Machine
{
public:
	MachineBottler();
	~MachineBottler();

	bool Initialise(Renderer& renderer) override;

};

#endif // MACHINEBOTTLER_H