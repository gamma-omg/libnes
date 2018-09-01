#include <memory.h>
#include "INESRom.h"

namespace nescore
{

const char INESRom::FORMAT[] = { 0x4E, 0x45, 0x53, 0x1A };

INESRom::INESRom()
    : _trainer(0)
    , _prgRom(0)
    , _chrRom(0)
    , _playChoice10(0)
{
    memset(&_header, 0x00, sizeof(INESHeader));
}

INESRom::~INESRom()
{
    clear();
}

void INESRom::read(std::istream &stream)
{
    clear();

    stream.read(reinterpret_cast<char*>(&_header.fileFormat), 4);
    if (memcmp(_header.fileFormat, FORMAT, 4) != 0)
    {
        throw nesformat_error("Invalid ROM format type");
    }

    stream >> _header.prgRomBanks;
    stream >> _header.chrRomBanks;
    stream >> _header.flag6;
    stream >> _header.flag7;
    stream >> _header.prgRamBanks;
    stream >> _header.flag9;
    stream.ignore(6);

    if (hasTrainer())
    {
        _trainer = new uint8_t[TRAINER_SIZE];
        stream.read(reinterpret_cast<char*>(_trainer), TRAINER_SIZE);
    }

    auto prgRomSize = _header.prgRomBanks * PRG_ROM_BANK_SIZE;
    if (prgRomSize > 0)
    {
        _prgRom = new uint8_t[prgRomSize];
        stream.read(reinterpret_cast<char*>(_prgRom), prgRomSize);
    }

    auto chrRomSize = _header.chrRomBanks * CHR_ROM_BANK_SIZE;
    if (chrRomSize)
    {
        _chrRom = new uint8_t[chrRomSize];
        stream.read(reinterpret_cast<char*>(_chrRom), chrRomSize);
    }

    if (hasPlayChoice10())
    {
        _playChoice10 = new uint8_t[PLAY_CHOICE_10_SIZE];
        stream.read(reinterpret_cast<char*>(_playChoice10), PLAY_CHOICE_10_SIZE);
    }
}

uint8_t INESRom::getMapper() const
{
    uint8_t l = _header.flag6 & Flag6::MAPPER_LOWER;
    uint8_t h = _header.flag7 & Flag7::MAPPER_UPPER;
    return h | (l >> 4);
}

INESRom::Mirroring INESRom::getMirroring() const
{
    return static_cast<Mirroring >(_header.flag9 & Flag6::MIRRORING);
}

INESRom::TVSystem INESRom::getTVSystem() const
{
    return static_cast<TVSystem>(_header.flag9 & Flag9::TV_SYSTEM);
}

bool INESRom::hasPersistentMemory() const
{
    return _header.flag6 & Flag6::PERSISTENT_MEMORY;
}

bool INESRom::hasTrainer() const
{
    return _header.flag6 & Flag6::TRAINER;
}

bool INESRom::getIgnoreMirroring() const
{
    return _header.flag6 & Flag6::IGNORE_MIRRORING;
}

bool INESRom::hasVSUnisystem() const
{
    return _header.flag7 & Flag7::VS_UNISYSTEM;
}

bool INESRom::hasPlayChoice10() const
{
    return _header.flag7 & Flag7::PLAY_CHOICE_10;
}

bool INESRom::isNES2Format() const
{
    return _header.flag7 & Flag7::NES2_FORMAT == 0b1000;
}

uint8_t INESRom::getPrgRomBanks() const
{
    return _header.prgRomBanks;
}

uint8_t INESRom::getChrRomBanks() const
{
    return _header.chrRomBanks;
}

uint8_t INESRom::getPrgRamBanks() const
{
    return _header.prgRamBanks;
}

uint8_t* INESRom::getTrainer() const
{
    return _trainer;
}

uint8_t* INESRom::getPrgRomBank(int bank) const
{
    return &_prgRom[bank * PRG_ROM_BANK_SIZE];
}

uint8_t* INESRom::getChrRomBank(int bank) const
{
    return &_chrRom[bank * CHR_ROM_BANK_SIZE];
}

uint8_t* INESRom::getPlayChoice10() const
{
    return _playChoice10;
}

uint8_t *INESRom::getPrgRom()
{
    return _prgRom;
}

uint8_t *INESRom::getChrRom()
{
    return _chrRom;
}

void INESRom::clear()
{
    if (_trainer) delete[] _trainer;
    if (_playChoice10) delete[] _playChoice10;
    if (_prgRom) delete[] _prgRom;
    if (_chrRom) delete[] _chrRom;

    _trainer = 0;
    _playChoice10 = 0;
    _prgRom = 0;
    _chrRom = 0;
    memset(&_header, 0x00, sizeof(INESHeader));
}

std::istream &operator>>(std::istream &stream, INESRom &rom)
{
    rom.read(stream);
    return stream;
}

}