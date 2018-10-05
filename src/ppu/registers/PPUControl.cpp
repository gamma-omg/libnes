#include "PPUControl.h"

namespace nescore
{

PPUControl::SpriteSize::SpriteSize(uint8_t width, uint8_t height)
    : width(width)
    , height(height)
{
}

PPUControl::PPUControl() : _register(0) {}

uint16_t PPUControl::getNametableAddr() const
{
    return 0x2000 + ((_register & Bits::NAMETABLE_ADDR) << 10);
}

uint16_t PPUControl::getSpritePatternAddr() const
{
    return (_register & Bits::SPRITE_PATTERN_ADDR) << 9;
}

uint16_t PPUControl::getBackgroundPatternAddr() const
{
    return (_register & Bits::BACKGROUND_PATTERN_ADDR) << 9;
}

uint8_t PPUControl::getVRAMIncrement() const
{
    auto mode = (_register & Bits::VRAM_INCREMENT) != 0;
    return mode ? 32 : 1;
}

bool PPUControl::getMasterSlave() const
{
    return _register & Bits::MASTER_SLAVE;
}

bool PPUControl::getGenerateNMI() const
{
    return _register & Bits::GENERATE_NMI;
}

PPUControl::SpriteSize PPUControl::getSpriteSize() const
{
    auto mode = (_register & Bits::SPRITE_SIZE) != 0;
    return mode ? SpriteSize(8, 8) : SpriteSize(16, 16);
}

PPUControl& PPUControl::operator=(uint8_t value)
{
    _register = value;
    return *this;
}

PPUControl::operator uint8_t() const
{
    return _register;
}

}
