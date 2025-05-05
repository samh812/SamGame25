#ifndef MACHINECAPPER_H
#define MACHINECAPPER_H

#include "machine.h"

class MachineCapper : public Machine {
public:
    MachineCapper();
    ~MachineCapper();

    bool Initialise(Renderer& renderer) override;
};

#endif // MACHINECAPPER_H