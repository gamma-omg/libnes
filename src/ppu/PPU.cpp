#include <memory.h>
#include "PPU.h"
#include "PPUMemory.h"
#include "../cpu/CPU.h"
#include "../memory/CPUMemory.h"

namespace nescore
{

PPU::PPU(std::shared_ptr<CPU> cpu)
    : _cpu(cpu)
    , _memory(new PPUMemory())
    , _registers(this)
    , _oamDma(this)
{
    _registers.mountTo(_cpu->getMemory());
    _oamDma.mountTo(_cpu->getMemory());

    memset(&_oam, 0x100, sizeof(uint8_t));
}

std::shared_ptr<PPUMemory> PPU::getMemory()
{
    return _memory;
}

void PPU::setPPUControl(uint8_t value)
{
    _ppuControl = value;
}

void PPU::setPPUMask(uint8_t value)
{
    _ppuMask = value;
}

void PPU::setPPUStatus(uint8_t value)
{
    _ppuStatus = value;
}

void PPU::setOamData(uint8_t value)
{
    _oam[_oamAddr++] = value;
}

void PPU::setOamAddr(uint8_t value)
{
    _oamAddr = value;
}

void PPU::setPPUScroll(uint8_t value)
{
    _ppuScroll = value;
}

void PPU::setPPUAddress(uint8_t value)
{
    _ppuAddress = value;
}

void PPU::setPPUData(uint8_t value)
{
    // TODO: implement
}

void PPU::setOamDma(uint8_t value)
{
    _cpu->getMemory()->readBytes(&_oam[_oamAddr], value << 8, 0xFF);
    _cpu->startDmaTransfer();
}

const PPUControl& PPU::getPPUControl() const
{
    return _ppuControl;
}

const PPUMask& PPU::getPPUMask() const
{
    return _ppuMask;
}

const PPUStatus& PPU::getPPUStatus() const
{
    return _ppuStatus;
}

uint8_t PPU::getOamAddr() const
{
    return _oamAddr;
}

uint8_t PPU::getOamData() const
{
    return _oam[_oamAddr];
}

uint8_t PPU::getPPUData() const
{
    // TODO: implement
    return 0;
}


}