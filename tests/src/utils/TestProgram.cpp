#include <fstream>
#include <rom/INESRom.h>
#include "TestProgram.h"

namespace nescore
{

TestProgram::TestProgram(const std::string &fileName)
    : _memory(new Memory)
    , _cpu(new CPU(_memory))
    , _mapperFactory(_memory)
    , _started(false)
{
    loadRom(fileName);
}

void TestProgram::run()
{
    // Set PPU status
    _memory->writeByte(0x2002, 0b10000000);
    _cpu->reset();

    while (true)
    {
        _cpu->tick();

        auto status = _memory->readByte(0x6000);
        if (!_started)
        {
            _started = status == 0x80;
            continue;
        }
        if (status == 0x00)
        {
            break;
        }
    }

    _output = _memory->readString(0x6004);
}

const std::string &TestProgram::getOutput() const
{
    return _output;
}

void TestProgram::loadRom(const std::string &fileName)
{
    std::ifstream fin;
    fin.open(fileName, std::ios::binary);
    INESRom rom;
    fin >> rom;

    auto mapper = _mapperFactory.createMapper(rom.getMapper());
    mapper->map(rom);
}

}