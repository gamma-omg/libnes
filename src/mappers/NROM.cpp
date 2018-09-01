#include <memory.h>
#include "NROM.h"
#include "../rom/INESRom.h"

namespace nescore
{

const Memory::Range NROM::PRG_RAM = Memory::Range(0x6000, 0x7FFF);
const Memory::Range NROM::PRG_ROM_1 = Memory::Range(0x8000, 0xBFFF);
const Memory::Range NROM::PRG_ROM_2 = Memory::Range(0xC000, 0xFFFF);

NROM::NROM(std::shared_ptr<INESRom> rom)
    : _rom(rom)
    , _prgRam(new uint8_t[0x2000])
{
}

NROM::~NROM()
{
    delete[] _prgRam;
}

void NROM::setupCPU(std::shared_ptr<Memory> memory)
{
    memory->mount(PRG_RAM, _prgRam);

    if (_rom->getPrgRomBanks() == 2)
    {
        memory->mount(PRG_ROM_1, _rom->getPrgRomBank(0));
        memory->mount(PRG_ROM_2, _rom->getPrgRomBank(1));
    }
    else
    {
        memory->mount(PRG_ROM_1, _rom->getPrgRomBank(0));
        memory->mount(PRG_ROM_2, _rom->getPrgRomBank(0));
    }
}

void NROM::setupPPU(std::shared_ptr<Memory> memory)
{
}

}
