#include "OamDmaAccessor.h"
#include "../PPU.h"

namespace nescore
{

const Memory::Range OamDmaAccessor::ADDRESS = Memory::Range(0x4014, 0x4014);

OamDmaAccessor::OamDmaAccessor(nescore::PPU *ppu)
    : _ppu(ppu)
{
}

void OamDmaAccessor::mountTo(std::shared_ptr<CPUMemory> memory)
{
    memory->mount(ADDRESS, this);
}

void OamDmaAccessor::writeByte(uint16_t offset, uint8_t value)
{
    _ppu->setOamDma(value);
}

uint8_t OamDmaAccessor::readByte(uint16_t offset)
{
    return 0;
}

}