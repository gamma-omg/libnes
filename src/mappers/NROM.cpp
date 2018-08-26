#include <memory.h>
#include "NROM.h"
#include "../Memory.h"
#include "../rom/INESRom.h"

namespace nescore
{

NROM::CPUMemory::CPUMemory(std::shared_ptr<INESRom> rom, std::shared_ptr<Memory> memory)
    : _rom(rom)
    , _memory(memory)
    , _prgRam(new uint8_t[0x2000])
{
    memset(_prgRam, 0x00, 0x2000);
    _prgRom = _rom->getPrgRom();
}

NROM::CPUMemory::~CPUMemory()
{
    delete[] _prgRam;
}

uint8_t NROM::CPUMemory::readByte(uint16_t offset)
{
    if (offset < 0x6000) return _memory->readByte(offset);
    if (offset < 0x8000) return _prgRam[offset - 0x6000];
    return _prgRom[offset - 0x8000];
}

void NROM::CPUMemory::writeByte(uint16_t offset, uint8_t value)
{
    if (offset < 0x6000)
    {
        _memory->writeByte(offset, value);
        return;
    }
    if (offset < 0x8000)
    {
        _prgRam[offset - 0x6000] = value;
        return;
    }

    _prgRom[offset - 0x8000] = value;
}

uint16_t NROM::CPUMemory::getStackOffset()
{
    return _memory->getStackOffset();
}

NROM::PPUMemory::PPUMemory(std::shared_ptr<INESRom> rom)
    : _rom(rom)
{
}

uint8_t NROM::PPUMemory::readByte(uint16_t offset)
{
    return 0;
}

void NROM::PPUMemory::writeByte(uint16_t offset, uint8_t value)
{

}

NROM::NROM(std::shared_ptr<INESRom> rom, std::shared_ptr<Memory> memory)
    : _cpuMemory(new CPUMemory(rom, memory))
    , _ppuMemory(new PPUMemory(rom))
{
}

std::shared_ptr<IMemoryAccessor> NROM::getCPUMemory()
{
    return _cpuMemory;
}

std::shared_ptr<IMemoryAccessor> NROM::getPPUMemory()
{
    return _ppuMemory;
}

}
