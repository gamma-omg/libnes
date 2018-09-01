#ifndef NESCORE_PPUCONTROL_H
#define NESCORE_PPUCONTROL_H

#include <cstdint>

namespace nescore
{

class PPUControl
{
public:
    struct SpriteSize
    {
        uint8_t width;
        uint8_t height;

        SpriteSize(uint8_t width, uint8_t height);
    };

private:
    enum Bits
    {
        NAMETABLE_ADDR = 0x00000011,
        VRAM_INCREMENT = 0x00000100,
        SPRITE_PATTERN_ADDR = 0x00001000,
        BACKGROUND_PATTERN_ADDR = 0x00010000,
        SPRITE_SIZE = 0x00100000,
        MASTER_SLAVE = 0x01000000,
        GENERATE_NMI = 0x100000000
    };

public:
    PPUControl();

    uint16_t getNametableAddr() const;
    uint16_t getSpritePatternAddr() const;
    uint16_t getBackgroundPatternAddr() const;
    uint8_t getVRAMIncrement() const;
    bool getMasterSlave() const;
    bool getGenerateNMI() const;
    SpriteSize getSpriteSize() const;

    PPUControl& operator=(uint8_t value);
    operator uint8_t() const;

private:
    uint8_t _register;
};

}

#endif //NESCORE_PPUCONTROL_H
