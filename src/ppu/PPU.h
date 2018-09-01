#ifndef NESCORE_PPU_H
#define NESCORE_PPU_H

#include <memory>
#include "registers/PPUControl.h"
#include "registers/PPUMask.h"
#include "registers/PPUStatus.h"
#include "registers/PPUScroll.h"
#include "registers/PPUAddress.h"
#include "registers/PPURegistersAccessor.h"
#include "registers/OamDmaAccessor.h"

namespace nescore
{

class CPU;

class PPU
{
public:
    PPU(std::shared_ptr<CPU> cpu);

    void setPPUControl(uint8_t value);
    void setPPUMask(uint8_t value);
    void setPPUStatus(uint8_t value);
    void setPPUScroll(uint8_t value);
    void setPPUAddress(uint8_t value);
    void setPPUData(uint8_t value);
    void setOamAddr(uint8_t value);
    void setOamData(uint8_t value);
    void setOamDma(uint8_t value);

    const PPUControl& getPPUControl() const;
    const PPUMask& getPPUMask() const ;
    const PPUStatus& getPPUStatus() const;
    uint8_t getPPUData() const;
    uint8_t getOamAddr() const;
    uint8_t getOamData() const;

private:
    std::shared_ptr<CPU> _cpu;

    PPURegistersAccessor _registers;
    OamDmaAccessor _oamDma;

    PPUControl _ppuControl;
    PPUMask _ppuMask;
    PPUStatus _ppuStatus;
    PPUScroll _ppuScroll;
    PPUAddress _ppuAddress;
    uint8_t _oamAddr;

    uint8_t _oam[0x100];
};

}

#endif //NESCORE_PPU_H
