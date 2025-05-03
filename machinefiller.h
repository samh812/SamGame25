#ifndef MACHINEFILLER_H
#define MACHINEFILLER_H

#include "machine.h"

class MachineFiller : public Machine {
public:
    MachineFiller();
    ~MachineFiller();

    bool Initialise(Renderer& renderer) override;
};

#endif // MACHINEFILLER_H