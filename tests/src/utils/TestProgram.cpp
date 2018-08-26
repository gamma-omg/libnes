#include <fstream>
#include <CPU.h>
#include <Memory.h>
#include <rom/INESRom.h>
#include "TestProgram.h"

namespace nescore
{

TestProgram::TestProgram(const std::string &fileName)
    : _memory(new Memory())
    , _cpu(nullptr)
    , _rom(nullptr)
    , _mapperFactory(_memory)
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
            _output = _memory->readString(0x6004);
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

    auto mapper = _mapperFactory.createMapper(_rom);
    _cpu = std::make_shared<CPU>(mapper->getCPUMemory());
}

}