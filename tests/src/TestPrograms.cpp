#include <gtest/gtest.h>
#include <fstream>
#include <rom/INESRom.h>
#include <mappers/MapperFactory.h>
#include <CPU.h>
#include <Memory.h>

using namespace nescore;

class TestRunner
{
public:
    TestRunner(const std::string& romFile)
        : _memory(new Memory())
        , _cpu(new CPU(_memory))
        , _started(false)
    {
        loadRom(romFile);
    }

    void run()
    {
        while (true)
        {
            _cpu->tick();

            auto status = _memory->readByte(0x6000);
            if (!_started && status == 0x80)
            {
                _started = true;
            }

            if (!_started)
            {
                continue;
            }

            if (_started && status == 0x00)
            {
                _output = _memory->readString(0x6004);
                return;
            }
        }
    }

    const std::string& getOutput() const { return _output; }

private:
    void loadRom(const std::string& romFile)
    {
        std::ifstream fin;
        fin.open(romFile, std::ios::binary);
        INESRom rom;
        fin >> rom;

        MapperFactory mapperFactory(_memory);
        auto mapper = mapperFactory.createMapper(rom.getMapper());
        mapper->map(rom);

        _memory->writeByte(0x2002, 0b10000000);
        _cpu->reset();
    }

private:
    std::shared_ptr<Memory> _memory;
    std::shared_ptr<CPU> _cpu;
    std::string _output;
    bool _started;
};

TEST(CPU, immediate)
{
    TestRunner testRunner("tests/data/02-immediate.nes");

    try
    {
        testRunner.run();
    }
    catch (std::exception e)
    {
        auto output = testRunner.getOutput();
        std::cout << output << std::endl;
    }

    auto output = testRunner.getOutput();
    std::cout << output << std::endl;
}

