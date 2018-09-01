#include "PPUMask.h"

namespace nescore
{

PPUMask::PPUMask() : _register(0)
{
}

bool PPUMask::getGrayscale() const
{
    return _register & Bits::GRAYSCALE;
}

bool PPUMask::getShowLeftBackground() const
{
    return _register & Bits::SHOW_LEFT_BACKGROUND;
}

bool PPUMask::getShowLeftSprites() const
{
    return _register & Bits::SHOW_LEFT_SPRITES;
}

bool PPUMask::getShowBackground() const
{
    return _register & Bits::SHOW_BACKGROUND;
}

bool PPUMask::getShowSprites() const
{
    return _register & Bits::SHOW_SPRITES;
}

bool PPUMask::getEmphasizeRed() const
{
    return _register & Bits::EMPHASIZE_RED;
}

bool PPUMask::getEmphasizeGreen() const
{
    return _register & Bits::EMPHASIZE_GREEN;
}

bool PPUMask::getEmphasizeBlue() const
{
    return _register & Bits::EMPHASIZE_BLUE;
}

PPUMask &PPUMask::operator=(uint8_t value)
{
    _register = value;
    return *this;
}

PPUMask::operator uint8_t() const
{
    return _register;
}

}