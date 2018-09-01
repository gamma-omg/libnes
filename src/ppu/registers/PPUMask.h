#ifndef NESCORE_PPUMASK_H
#define NESCORE_PPUMASK_H

#include <cstdint>

namespace nescore
{

class PPUMask
{
private:
    enum Bits
    {
        GRAYSCALE = 0x00000001,
        SHOW_LEFT_BACKGROUND = 0x00000010,
        SHOW_LEFT_SPRITES = 0x00000100,
        SHOW_BACKGROUND = 0x0001000,
        SHOW_SPRITES = 0x00010000,
        EMPHASIZE_RED = 0x00100000,
        EMPHASIZE_GREEN = 0x01000000,
        EMPHASIZE_BLUE = 0x10000000,
    };

public:
    PPUMask();

    bool getGrayscale() const;
    bool getShowLeftBackground() const;
    bool getShowLeftSprites() const;
    bool getShowBackground() const;
    bool getShowSprites() const;
    bool getEmphasizeRed() const;
    bool getEmphasizeGreen() const;
    bool getEmphasizeBlue () const;

    PPUMask& operator=(uint8_t value);
    operator uint8_t() const;

private:
    uint8_t _register;
};

}

#endif //NESCORE_PPUMASK_H
