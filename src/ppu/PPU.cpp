#include <memory.h>
#include "PPU.h"
#include "PPUMemory.h"
#include "IRenderCallback.h"
#include "../cpu/CPU.h"
#include "../cpu/CPUMemory.h"

namespace nescore
{

PPU::PPU(std::shared_ptr<CPU> cpu)
    : _cpu(cpu)
    , _memory(new PPUMemory())
    , _registers(this)
    , _oamDma(this)
    , _renderer(SCREEN_WIDTH, SCREEN_HEIGHT)
    , _lineX(0)
    , _lineY(0)
    , _vramReadBuffer(0)
{
    _registers.mountTo(_cpu->getMemory());
    _oamDma.mountTo(_cpu->getMemory());

    memset(&_oam, 0x100, sizeof(uint8_t));
}

void PPU::setRenderCallback(IRenderCallback *callback)
{
    _renderCallback = callback;
}

std::shared_ptr<PPUMemory> PPU::getMemory()
{
    return _memory;
}

void PPU::update(master_cycle time)
{
    _masterClock = time;
    while (_ppuClock < _masterClock)
    {
        tick();
    }
}

void PPU::tick()
{
    if (_lineX % 8 == 0)
    {
//        _renderer.setPattern(readPattern(), _lineY % 8);
//        _renderer.setAttributes(readAttributes());
//        _renderer.render(_lineX, _lineY);
    }

    _lineX++;
    if (_lineX > _renderer.getWidth())
    {
        _lineX = 0;
        _lineY++;
    }

    if (_lineY > _renderer.getHeight())
    {
        _lineY = 0;
        _lineX = 0;
        _renderer.setPattersSource(_memory.get());
        _renderer.renderPatternTables();

        if (_renderCallback)
        {
            _renderCallback->renderFrame(_renderer.getWidth(), _renderer.getHeight(), _renderer.getOutput());
        }
    }

    _ppuClock++;
}

void PPU::reset()
{
    // TODO: implement
}

void PPU::setPPUControl(uint8_t value)
{
    _ppuControl = value;
    _latch = value;
}

void PPU::setPPUMask(uint8_t value)
{
    _ppuMask = value;
    _latch = value;
}

void PPU::setPPUStatus(uint8_t value)
{
    _ppuStatus = value;
    _latch = value;
}

void PPU::setOamData(uint8_t value)
{
    _oam[_oamAddr++] = value;
    _latch = value;
}

void PPU::setOamAddr(uint8_t value)
{
    _oamAddr = value;
    _latch = value;
}

void PPU::setPPUScroll(uint8_t value)
{
    _ppuScroll = value;
    _latch = value;
}

void PPU::setPPUAddress(uint8_t value)
{
    _ppuAddress = value;
    _latch = value;
}

void PPU::setPPUData(uint8_t value)
{
    _memory->writeByte(_ppuAddress, value);
    _ppuAddress += _ppuControl.getVRAMIncrement();
    _latch = value;
}

void PPU::setOamDma(uint8_t value)
{
    _cpu->getMemory()->readBytes(&_oam[_oamAddr], value << 8, 0xFF);
    _cpu->startDmaTransfer();
}

void PPU::setLatch(uint8_t value)
{
    _latch = value;
}

const PPUControl& PPU::getPPUControl()
{
    return _ppuControl;
}

const PPUMask& PPU::getPPUMask()
{
    return _ppuMask;
}

const PPUStatus& PPU::getPPUStatus()
{
    _latch = _ppuStatus;
    return _ppuStatus;
}

uint8_t PPU::getOamAddr()
{
    return _oamAddr;
}

uint8_t PPU::getOamData()
{
    auto data = _oam[_oamAddr];
    _latch = data;

    return _oam[_oamAddr];
}

uint8_t PPU::getPPUData()
{
    auto data = _vramReadBuffer;
    _vramReadBuffer = _memory->readByte(_ppuAddress);
    _ppuAddress += _ppuControl.getVRAMIncrement();
    _latch = data;

    return data;
}

uint8_t PPU::getLatch() const
{
    return _latch;
}


}