#ifndef NESCORE_NES_H
#define NESCORE_NES_H

#include <memory>
#include <string>
#include "Timings.h"

namespace nescore
{

class CPU;
class PPU;
class INESRom;
class IRomMapper;

class NES
{
public:
    NES();

    void update(seconds delta);
    void loadRom(const std::string& fileName);
    void loadRom(std::shared_ptr<INESRom> rom);
    bool isRomLoaded() const;

private:
    std::shared_ptr<CPU> _cpu;
    std::shared_ptr<PPU> _ppu;
    std::shared_ptr<INESRom> _rom;
    std::shared_ptr<IRomMapper> _mapper;

    master_cycle _clock;

};

}

#endif //NESCORE_NES_H
