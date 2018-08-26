#include "PPU.h"

namespace nescore
{

PPU::Control::SpriteSize::SpriteSize(uint8_t width, uint8_t height)
    : width(width)
    , height(height)
{
}

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

}