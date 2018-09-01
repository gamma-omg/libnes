#include "PPURegistersAccessor.h"
#include "../PPU.h"

namespace nescore
{

const Memory::Range PPURegistersAccessor::RANGE = Memory::Range(0x2000, 0x2007);
const Memory::Range PPURegistersAccessor::MIRROR = Memory::Range(0x2008, 0x3FFF);

PPURegistersAccessor::PPURegistersAccessor(nescore::PPU* ppu)
    : _ppu(ppu)
{
}

void PPURegistersAccessor::writeByte(uint16_t offset, uint8_t value)
{
    switch (offset)
    {
        case PPUCTRL: _ppu->setPPUControl(value); return;
        case PPUMASK: _ppu->setPPUMask(value); return;
        case PPUSTATUS: _ppu->setPPUStatus(value); return;
        case OAMADDR: _ppu->setOamAddr(value); return;
        case OAMDATA: _ppu->setOamData(value); return;
        case PPUSCROLL: _ppu->setPPUScroll(value); return;
        case PPUADDR: _ppu->setPPUAddress(value); return;
        case PPUDATA: _ppu->setPPUData(value); return;
    }
}

uint8_t PPURegistersAccessor::readByte(uint16_t offset)
{
    switch (offset)
    {
        case PPUCTRL: return _ppu->getPPUControl();
        case PPUMASK: return _ppu->getPPUMask();
        case PPUSTATUS: return _ppu->getPPUStatus();
        case OAMADDR: return _ppu->getOamAddr();
        case OAMDATA: return _ppu->getOamData();
        case PPUDATA: return _ppu->getPPUData();
    }

    return 0;
}

void PPURegistersAccessor::mountTo(std::shared_ptr<CPUMemory> memory)
{
    memory->mount(RANGE, this);
    memory->mirror(RANGE, MIRROR);
}

}