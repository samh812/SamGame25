#ifndef MACHINELABELER_H
#define MACHINELABELER_H

#include "machine.h"

class MachineLabeler : public Machine {
public:
    MachineLabeler();
    ~MachineLabeler();

    bool Initialise(Renderer& renderer) override;
};

#endif // MACHINECAPPER_H