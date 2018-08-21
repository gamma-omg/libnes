#include <memory.h>
#include "INESRom.h"

namespace nescore
{

const char INESRom::FORMAT[] = { 0x4E, 0x45, 0x53, 0x1A };

INESRom::INESRom()
    : _trainer(0)
    , _prgRomBanks(0)
    , _chrRomBanks(0)
    , _playChoice10(0)
{
    memset(&_header, sizeof(INESHeader), 0x00);
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

    _prgRomBanks = readBankedMemory(stream, _header.prgRomBanks, PRG_ROM_BANK_SIZE);
    _chrRomBanks = readBankedMemory(stream, _header.chrRomBanks, CHR_ROM_BANK_SIZE);

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

const uint8_t* INESRom::getTrainer() const
{
    return _trainer;
}

const uint8_t* INESRom::getPrgRomBank(int bank) const
{
    return _prgRomBanks[bank];
}

const uint8_t* INESRom::getChrRomBank(int bank) const
{
    return _chrRomBanks[bank];
}

const  uint8_t* INESRom::getPlayChoice10() const
{
    return _playChoice10;
}

void INESRom::clear()
{
    if (_trainer) delete[] _trainer;
    if (_playChoice10) delete[] _playChoice10;

    deleteBankedMemory(_prgRomBanks, _header.prgRomBanks);
    deleteBankedMemory(_chrRomBanks, _header.chrRomBanks);

    memset(&_header, sizeof(INESHeader), 0x00);
}

void INESRom::deleteBankedMemory(uint8_t **ptr, int size)
{
    if (ptr == 0) return;

    for (int i = 0; i < size; ++i)
    {
        delete[] ptr[i];
    }
    delete[] ptr;
}

uint8_t** INESRom::readBankedMemory(std::istream &stream, int banks, int bankSize)
{
    uint8_t **ptr = new uint8_t *[banks];
    for (int i = 0; i < banks; ++i)
    {
        ptr[i] = new uint8_t[bankSize];
        stream.read(reinterpret_cast<char*>(ptr[i]), bankSize);
    }

    return ptr;
}

std::istream &operator>>(std::istream &stream, INESRom &rom)
{
    rom.read(stream);
    return stream;
}

}