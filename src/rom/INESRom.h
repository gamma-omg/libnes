#ifndef NESCORE_INESLOADER_H
#define NESCORE_INESLOADER_H

#include <stdint-gcc.h>
#include <memory>
#include <istream>

namespace nescore
{

class nesformat_error : std::runtime_error
{
public:
    nesformat_error(const std::string &message) : std::runtime_error(message) {}
};

class INESRom
{
public:
    static const char FORMAT[4];
    static const uint16_t TRAINER_SIZE = 0x200;
    static const uint16_t PRG_ROM_BANK_SIZE = 0x4000;
    static const uint16_t CHR_ROM_BANK_SIZE = 0x2000;
    static const uint16_t PLAY_CHOICE_10_SIZE = 0x2000;
    static const uint16_t PRG_RAM_BANK_SIZE = 0x2000;

public:

    struct INESHeader
    {
        char fileFormat[4];
        uint8_t prgRomBanks;
        uint8_t chrRomBanks;
        uint8_t flag6;
        uint8_t flag7;
        uint8_t prgRamBanks;
        uint8_t flag9;
    };

    enum Mirroring
    {
        HORIZONTAL = 0,
        VERTICAL = 1
    };

    enum TVSystem
    {
        NTSC,
        PAL,
    };

public:
    INESRom();
    INESRom(const INESRom&) = delete;
    ~INESRom();

    void read(std::istream& stream);

    Mirroring getMirroring() const;
    TVSystem getTVSystem() const;
    uint8_t getMapper() const;
    uint8_t getPrgRomBanks() const;
    uint8_t getChrRomBanks() const;
    uint8_t getPrgRamBanks() const;

    const uint8_t* getTrainer() const;
    const uint8_t* getPrgRomBank(int bank) const;
    const uint8_t* getChrRomBank(int bank) const;
    const uint8_t* getPlayChoice10() const;

    bool hasPersistentMemory() const;
    bool hasTrainer() const;
    bool getIgnoreMirroring() const;
    bool hasVSUnisystem() const;
    bool hasPlayChoice10() const;
    bool isNES2Format() const;

private:
    enum Flag6
    {
        MIRRORING = 0b00000001,
        PERSISTENT_MEMORY = 0b00000010,
        TRAINER = 0b00000100,
        IGNORE_MIRRORING = 0b00001000,
        MAPPER_LOWER = 0b11110000
    };

    enum Flag7
    {
        VS_UNISYSTEM = 0b00000001,
        PLAY_CHOICE_10 = 0b00000010,
        NES2_FORMAT = 0b00001100,
        MAPPER_UPPER = 0b11110000
    };

    enum Flag9
    {
        TV_SYSTEM = 0b00000001
    };

private:
    void clear();
    void deleteBankedMemory(uint8_t** ptr, int size);
    uint8_t** readBankedMemory(std::istream& stream, int banks, int bankSize);

private:
    INESHeader _header;
    uint8_t* _trainer;
    uint8_t* _playChoice10;
    uint8_t** _prgRomBanks;
    uint8_t** _chrRomBanks;
};

std::istream& operator >>(std::istream& stream, INESRom& rom);

}

#endif //NESCORE_INESLOADER_H
