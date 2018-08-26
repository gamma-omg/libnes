#ifndef NESCORE_PPU_H
#define NESCORE_PPU_H

#include <stdint-gcc.h>

namespace nescore
{

class PPU
{
public:
    static const uint16_t PPUCTRL = 0x2000;
    static const uint16_t PPUMASK = 0x2001;
    static const uint16_t PPUSTATUS = 0x2002;
    static const uint16_t OAMADDR = 0x2003;
    static const uint16_t OAMDATA = 0x2004;
    static const uint16_t PPUSCROLL = 0x2005;
    static const uint16_t PPUADDR = 0x2006;

public:
    class Control
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

    class Scroll
    {
    public:
        Scroll();

        uint8_t getX() const;
        uint8_t getY() const;

        Scroll& operator=(uint8_t value);
        operator uint8_t() const;

    private:
        uint8_t _scrolls[2];
        uint8_t _component;
    };

    class Address
    {
    public:
        Address();

        Address& operator=(uint8_t value);
        operator uint16_t() const;

    private:
        uint16_t _register;
        uint16_t _writeMask;
    };

public:

    void setPPUControl(uint8_t value);
    void setPPUMask(uint8_t value);
    void setPPUStatus(uint8_t value);
    void setPPUScroll(uint8_t value);
    void setPPUAddress(uint8_t value);
    void setOamAddr(uint8_t value);
    void setOamData(uint8_t value);

    const Control& getPPUControl() const;
    const Mask& getPPUMask() const ;
    const Status& getPPUStatus() const;
    const Scroll& getPPUScroll() const;
    const Address& getPPUAddress() const;
    uint8_t getOamAddr() const;
    uint8_t getOamData() const;

private:
    Control _ppuControl;
    Mask _ppuMask;
    Status _ppuStatus;
    Scroll _ppuScroll;
    Address _ppuAddress;
    uint8_t _oamAddr;

};

}

#endif //NESCORE_PPU_H
