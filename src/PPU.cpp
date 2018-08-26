#include "PPU.h"

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


PPU::Control& PPU::getPPUControl()
{
    return _ppuControl;
}

PPU::Mask &PPU::getPPUMask()
{
    return <#initializer#>_ppuMask;
}

}