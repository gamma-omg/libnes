#ifndef NESCORE_PPU_H
#define NESCORE_PPU_H

#include <stdint-gcc.h>

namespace nescore
{

class PPU
{
public:
    class Control
    {
    public:
        static const uint16_t ADDR = 0x2000;

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
        Control();

        uint16_t getNametableAddr() const;
        uint16_t getSpritePatternAddr() const;
        uint16_t getBackgroundPatternAddr() const;
        uint8_t getVRAMIncrement() const;
        bool getMasterSlave() const;
        bool getGenerateNMI() const;
        SpriteSize getSpriteSize() const();

        Control& operator=(uint8_t value);
        operator uint8_t() const;

    private:
        uint8_t _register;
    };

    class Mask
    {
    public:
        static const uint16_t ADDR = 0x2001;

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
        Mask();

        bool getGrayscale() const;
        bool getShowLeftBackground() const;
        bool getShowLeftSprites() const;
        bool getShowBackground() const;
        bool getShowSprites() const;
        bool getEmphasizeRed() const;
        bool getEmphasizeGreen() const;
        bool getEmphasizeBlue () const;

        Mask& operator=(uint8_t value);
        operator uint8_t() const;

    private:
        uint8_t _register;
    };

    class Status
    {
    public:
        static const uint16_t ADDR = 0x2002;

    private:
        enum Bits
        {
            SPRITE_OVERFLOW = 0x00100000,
            SPRITE_0_HIT = 0x01000000,
            VBLANK = 0x10000000
        };

    public:
        Status();

        void setSpriteOverflow(bool value);
        void setSprite0Hit(bool value);
        void setVBlank(bool value);

        Status& operator=(uint8_t value);
        operator uint8_t() const;

    private:
        uint8_t _register;
    };

public:

    void setPPUControl(uint8_t value);
    void setPPUMask(uint8_t value);
    void setPPUStatus(uint8_t value);

    const Control& getPPUControl() const;
    const Mask& getPPUMask() const ;
    const Status& getPPUStatus() const;

private:
    Control _ppuControl;
    Mask _ppuMask;
    Status _ppuStatus;

};

}

#endif //NESCORE_PPU_H
