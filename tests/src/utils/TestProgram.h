#ifndef NESCORE_TESTPROGRAM_H
#define NESCORE_TESTPROGRAM_H

#include <string>
#include <CPU.h>
#include <Memory.h>
#include <mappers/MapperFactory.h>

namespace nescore
{

class TestProgram
{
public:
    TestProgram(const std::string& fileName);

    int run();
    const std::string& getOutput() const;

private:
    void loadRom(const std::string& fileName);

private:
    std::shared_ptr<Memory> _memory;
    std::shared_ptr<CPU> _cpu;
    std::string _output;
    MapperFactory _mapperFactory;
    bool _started;
};

}


#endif //NESCORE_TESTPROGRAM_H
