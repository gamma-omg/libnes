#include <fstream>
#include <cpu/CPU.h>
#include <memory/CPUMemory.h>
#include <rom/INESRom.h>
#include "TestProgram.h"

using namespace nescore;

TestProgram::TestProgram(const std::string &fileName)
    : _cpu(new CPU())
    , _rom(nullptr)
    , _mapper(nullptr)
    , _started(false)
{
    loadRom(fileName);
}

int TestProgram::run()
{
    // Set PPU status
    _cpu->getMemory()->writeByte(0x2002, 0b10000000);
    _cpu->reset();

    while (true)
    {
        _cpu->tick();

        auto status = _cpu->getMemory()->readByte(0x6000);
        if (!_started)
        {
            _started = status == 0x80;
            continue;
        }
        if (status != 0x80)
        {
            _output = _cpu->getMemory()->readString(0x6004);
            return status;
        }
    }
}

const std::string &TestProgram::getOutput() const
{
    return _output;
}

void TestProgram::loadRom(const std::string &fileName)
{
    _rom = std::make_shared<INESRom>();

    std::ifstream fin;
    fin.open(fileName, std::ios::binary);
    _rom->read(fin);

    _mapper = _mapperFactory.createMapper(_rom);
    _mapper->setupCPU(_cpu->getMemory());
}