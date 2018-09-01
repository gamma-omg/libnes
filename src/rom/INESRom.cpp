#include <memory.h>
#include "INESRom.h"

namespace nescore
{

INESRom::Bank::Bank(uint16_t size)
    : _size(size)
    , _data(nullptr)
{
}

INESRom::Bank::Bank(INESRom::Bank &&other)
{
    _size = other._size;
    _data = other._data;
    other._data = nullptr;
}

INESRom::Bank::~Bank()
{
    clear();
}

uint16_t INESRom::Bank::getSize() const
{
    return _size;
}

const uint8_t *INESRom::Bank::getData() const
{
    return _data;
}

uint8_t INESRom::Bank::getByte(uint16_t offset) const
{
    return _data[offset];
}

void INESRom::Bank::read(std::istream &stream)
{
    clear();

    _data = new uint8_t[_size];
    stream.read(reinterpret_cast<char*>(_data), _size);
}

void INESRom::Bank::clear()
{
    if (_data) delete[] _data;
}

const char INESRom::FORMAT[] = { 0x4E, 0x45, 0x53, 0x1A };

INESRom::INESRom()
    : _trainer(TRAINER_SIZE)
    , _playChoice10(PLAY_CHOICE_10_SIZE)
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
        stream >> _trainer;
    }

    for (int i = 0; i < _header.prgRomBanks; ++i)
    {
        auto bank = new Bank(PRG_ROM_BANK_SIZE);
        stream >> *bank;
        _prgRoms.push_back(bank);
    }

    for (int i = 0; i < _header.chrRomBanks; ++i)
    {
        auto bank = new Bank(CHR_ROM_BANK_SIZE);
        stream >> *bank;
        _chrRoms.push_back(bank);
    }

    if (hasPlayChoice10())
    {
        stream >> _playChoice10;
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

const INESRom::Bank& INESRom::getTrainer() const
{
    return _trainer;
}

const INESRom::Bank& INESRom::getPrgRomBank(int bank) const
{
    return *_prgRoms[bank];
}

const INESRom::Bank& INESRom::getChrRomBank(int bank) const
{
    return *_chrRoms[bank];
}

const INESRom::Bank& INESRom::getPlayChoice10() const
{
    return _playChoice10;
}

void INESRom::clear()
{
    for (auto& bank : _prgRoms) delete bank;
    for (auto& bank : _chrRoms) delete bank;

    _playChoice10.clear();
    _trainer.clear();
    _prgRoms.clear();
    _chrRoms.clear();

    memset(&_header, 0x00, sizeof(INESHeader));
}

std::istream &operator>>(std::istream& stream, INESRom& rom)
{
    rom.read(stream);
    return stream;
}

std::istream &operator>>(std::istream& stream, INESRom::Bank& bank)
{
    bank.read(stream);
    return stream;
}

}