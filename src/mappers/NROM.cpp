#include "NROM.h"
#include "../Memory.h"
#include "../rom/INESRom.h"

namespace nescore
{

NROM::NROM(std::shared_ptr<Memory> memory) : IRomMapper(memory)
{
}

void NROM::map(nescore::INESRom &rom)
{
    _memory->writeBytes(0x8000, rom.getPrgRomBank(0), INESRom::PRG_ROM_BANK_SIZE);
    _memory->writeBytes(0xC000, rom.getPrgRomBank(1), INESRom::PRG_ROM_BANK_SIZE);
}

}
