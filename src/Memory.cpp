#include <stdexcept>
#include <memory.h>
#include "Memory.h"
#include "PPU.h"

namespace nescore
{

Memory::Memory() : Memory(nullptr)
{
}

Memory::Memory(std::shared_ptr<PPU> ppu)
    : _ppu(ppu)
    , _memory(new uint8_t[0x10000])
{
    memset(_memory, 0x00, 0x10000);
}

Memory::Memory(const std::vector<uint8_t> &ram)
    : Memory()
{
    uint8_t* address = _memory;
    memcpy(address, ram.data(), ram.size()); address += RAM_PAGE_SIZE;
    memcpy(address, ram.data(), ram.size()); address += RAM_PAGE_SIZE;
    memcpy(address, ram.data(), ram.size()); address += RAM_PAGE_SIZE;
    memcpy(address, ram.data(), ram.size());
}

Memory::~Memory()
{
    delete[] _memory;
}

void Memory::loadProgram(const std::vector<uint8_t> &program)
{
    memcpy(_memory + ROM_OFFSET, program.data(), program.size());
    writeShort(RESET_VECTOR, ROM_OFFSET);
}

void Memory::writeByte(uint16_t offset, uint8_t value)
{
    if (_ppu != nullptr)
    {
        switch (offset)
        {
            case PPU::PPUCTRL: _ppu->setPPUControl(value); return;
            case PPU::PPUMASK: _ppu->setPPUMask(value); return;
            case PPU::PPUSTATUS: _ppu->setPPUStatus(value); return;
            case PPU::OAMADDR: _ppu->setOamAddr(value); return;
            case PPU::OAMDATA: _ppu->setOamData(value); return;
            case PPU::PPUSCROLL: _ppu->setPPUScroll(value); return;
            case PPU::PPUADDR: _ppu->setPPUAddress(value); return;
            case PPU::PPUDATA: _ppu->setPPUData(value); return;
        }
    }

    if (offset < RAM_SIZE)
    {
        uint16_t address = offset % RAM_PAGE_SIZE;
        _memory[address] = value; address += RAM_PAGE_SIZE;
        _memory[address] = value; address += RAM_PAGE_SIZE;
        _memory[address] = value; address += RAM_PAGE_SIZE;
        _memory[address] = value;
        return;
    }

    _memory[offset] = value;
}

uint8_t Memory::readByte(uint16_t offset)
{
    if (_ppu != nullptr)
    {
        switch (offset)
        {
            case PPU::PPUCTRL: return _ppu->getPPUControl();
            case PPU::PPUMASK: return _ppu->getPPUMask();
            case PPU::PPUSTATUS: return _ppu->getPPUStatus();
            case PPU::OAMADDR: return _ppu->getOamAddr();
            case PPU::OAMDATA: return _ppu->getOamData();
            case PPU::PPUDATA: return _ppu->getPPUData();
        }
    }

    return _memory[offset];
}

std::string Memory::readString(uint16_t offset)
{
    return std::string(reinterpret_cast<char*>(_memory + offset));
}

uint16_t Memory::getStackOffset()
{
    return STACK_BOTTOM;
}

}