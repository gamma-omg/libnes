#include "NROM.h"
#include "../Memory.h"
#include "../rom/INESRom.h"

namespace nescore
{

void NROM::map(nescore::INESRom &rom)
{
    _memory->writeBytes(0x8000, rom.getPrgRomBank(0), INESRom::PRG_ROM_BANK_SIZE);
}

}
