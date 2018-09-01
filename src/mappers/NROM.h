#ifndef NESCORE_NROM_H
#define NESCORE_NROM_H

#include "IRomMapper.h"
#include "../memory/Memory.h"

namespace nescore
{

class NROM : public IRomMapper
{
public:
    static const Memory::Range PRG_RAM;
    static const Memory::Range PRG_ROM_1;
    static const Memory::Range PRG_ROM_2;

public:
    NROM(std::shared_ptr<INESRom> rom);
    ~NROM();

    void setupCPU(std::shared_ptr<Memory> memory) override;
    void setupPPU(std::shared_ptr<Memory> memory) override;

private:
    std::shared_ptr<INESRom> _rom;
    uint8_t* _prgRam;

};

}

#endif //NESCORE_NROM_H
