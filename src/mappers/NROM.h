#ifndef NESCORE_NROM_H
#define NESCORE_NROM_H

#include "IRomMapper.h"

namespace nescore
{

class NROM : public IRomMapper
{
public:
    void map(INESRom &rom) override;
};

}

#endif //NESCORE_NROM_H
