#ifndef MACHINECONVEYOR_H
#define MACHINECONVEYOR_H

#include "machine.h"

class MachineConveyor : public Machine {
public:
    MachineConveyor();
    ~MachineConveyor();

    bool Initialise(Renderer& renderer) override;
};

#endif // MACHINETYPECONVEYOR_H