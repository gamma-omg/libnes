#include <fstream>
#include "NES.h"
#include "cpu/CPU.h"
#include "cpu/CPUMemory.h"
#include "ppu/PPU.h"
#include "ppu/PPUMemory.h"
#include "rom/INESRom.h"
#include "mappers/MapperFactory.h"

namespace nescore
{

NES::NES()
    : _cpu(new CPU())
    , _ppu(new PPU(_cpu))
{
}

void NES::update(seconds delta)
{
    //_clock += delta;

    //_cpu->update(_clock);
    _ppu->update(_clock);
}

void NES::setRenderCallback(IRenderCallback *callback)
{
    _ppu->setRenderCallback(callback);
}

void NES::loadRom(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    std::shared_ptr<INESRom> rom = std::make_shared<INESRom>();
    rom->read(file);
    loadRom(rom);
}

void NES::loadRom(std::shared_ptr<INESRom> rom)
{
    _cpu->reset();
    _ppu->reset();

    _rom = rom;
    _mapper = MapperFactory::createMapper(rom);
    _mapper->setupCPU(_cpu->getMemory());
    _mapper->setupPPU(_ppu->getMemory());

    _clock = master_cycle(0);
}

bool NES::isRomLoaded() const
{
    return _rom != nullptr;
}

}