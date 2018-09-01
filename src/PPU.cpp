#include <memory.h>
#include "PPU.h"
#include "CPU.h"
#include "memory/CPUMemory.h"

namespace nescore
{

PPU::Control::SpriteSize::SpriteSize(uint8_t width, uint8_t height)
    : width(width)
    , height(height)
{
}

PPU::Control::Control() : _register(0) {}

uint16_t PPU::Control::getNametableAddr() const
{
    auto mode = _register & Bits::NAMETABLE_ADDR;
    switch (mode)
    {
        case 0: return 0x2000;
        case 1: return 0x2400;
        case 2: return 0x2800;
        case 3: return 0x2C00;
    }

    return 0;
}

uint16_t PPU::Control::getSpritePatternAddr() const
{
    auto mode = (_register & Bits::SPRITE_PATTERN_ADDR) != 0;
    return mode ? 0x1000 : 0x0000;
}

uint16_t PPU::Control::getBackgroundPatternAddr() const
{
    auto mode = (_register & Bits::BACKGROUND_PATTERN_ADDR) != 0;
    return mode ? 0x1000 : 0x0000;
}

uint8_t PPU::Control::getVRAMIncrement() const
{
    auto mode = (_register & Bits::VRAM_INCREMENT) != 0;
    return mode ? 32 : 1;
}

bool PPU::Control::getMasterSlave() const
{
    return _register & Bits::MASTER_SLAVE;
}

bool PPU::Control::getGenerateNMI() const
{
    return _register & Bits::GENERATE_NMI;
}

PPU::Control::SpriteSize PPU::Control::getSpriteSize() const
{
    auto mode = (_register & Bits::SPRITE_SIZE) != 0;
    return mode ? SpriteSize(8, 8) : SpriteSize(16, 16);
}

PPU::Control& PPU::Control::operator=(uint8_t value)
{
    _register = value;
    return *this;
}

PPU::Control::operator uint8_t() const
{
    return _register;
}


PPU::Mask::Mask() : _register(0)
{
}

bool PPU::Mask::getGrayscale() const
{
    return _register & Bits::GRAYSCALE;
}

bool PPU::Mask::getShowLeftBackground() const
{
    return _register & Bits::SHOW_LEFT_BACKGROUND;
}

bool PPU::Mask::getShowLeftSprites() const
{
    return _register & Bits::SHOW_LEFT_SPRITES;
}

bool PPU::Mask::getShowBackground() const
{
    return _register & Bits::SHOW_BACKGROUND;
}

bool PPU::Mask::getShowSprites() const
{
    return _register & Bits::SHOW_SPRITES;
}

bool PPU::Mask::getEmphasizeRed() const
{
    return _register & Bits::EMPHASIZE_RED;
}

bool PPU::Mask::getEmphasizeGreen() const
{
    return _register & Bits::EMPHASIZE_GREEN;
}

bool PPU::Mask::getEmphasizeBlue() const
{
    return _register & Bits::EMPHASIZE_BLUE;
}

PPU::Mask &PPU::Mask::operator=(uint8_t value)
{
    _register = value;
    return *this;
}

PPU::Mask::operator uint8_t() const
{
    return _register;
}


PPU::Status::Status() : _register(0)
{
}

void PPU::Status::setSpriteOverflow(bool value)
{
    auto bit = value ? 1 : 0;
    _register ^= (-bit ^ _register) & Bits::SPRITE_OVERFLOW;
}

void PPU::Status::setSprite0Hit(bool value)
{
    auto bit = value ? 1 : 0;
    _register ^= (-bit ^ _register) & Bits::SPRITE_0_HIT;
}

void PPU::Status::setVBlank(bool value)
{
    auto bit = value ? 1 : 0;
    _register ^= (-bit ^ _register) & Bits::VBLANK;
}

PPU::Status &PPU::Status::operator=(uint8_t value)
{
    _register = value;
    return *this;
}

PPU::Status::operator uint8_t() const
{
    return _register;
}


PPU::Scroll::Scroll() : _component(0)
{
    _scrolls[0] = 0;
    _scrolls[1] = 0;
}

uint8_t PPU::Scroll::getX() const
{
    return _scrolls[0];
}

uint8_t PPU::Scroll::getY() const
{
    return _scrolls[1];
}

PPU::Scroll &PPU::Scroll::operator=(uint8_t value)
{
    _scrolls[_component] = value;
    _component = (_component + 1) % 2;
    return *this;
}

PPU::Address::Address() : _register(0), _writeMask(0xFF00)
{
}

PPU::Address &PPU::Address::operator=(uint8_t value)
{
    _register = value & _writeMask;
    _writeMask = _writeMask > 0xFF ? _writeMask >> 8 : 0xFF00;
    return *this;
}

PPU::Address::operator uint16_t() const
{
    return _register;
}


PPU::PPU(std::shared_ptr<CPU> cpu)
    : _cpu(cpu)
{
    memset(&_oam, 0x100, sizeof(uint8_t));
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

const PPU::Control& PPU::getPPUControl() const
{
    return _ppuControl;
}

const PPU::Mask& PPU::getPPUMask() const
{
    return _ppuMask;
}

const PPU::Status& PPU::getPPUStatus() const
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