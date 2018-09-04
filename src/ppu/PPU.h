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
#include "../Timings.h"
#include "Renderer.h"

namespace nescore
{

class CPU;
class PPUMemory;

class PPU
{
public:
    static const int SCREEN_WIDTH = 256;
    static const int SCREEN_HEIGHT = 256;

public:
    PPU(std::shared_ptr<CPU> cpu);

    std::shared_ptr<PPUMemory> getMemory();

    void update(master_cycle time);
    void tick();
    void reset();

    void setPPUControl(uint8_t value);
    void setPPUMask(uint8_t value);
    void setPPUStatus(uint8_t value);
    void setPPUScroll(uint8_t value);
    void setPPUAddress(uint8_t value);
    void setPPUData(uint8_t value);
    void setOamAddr(uint8_t value);
    void setOamData(uint8_t value);
    void setOamDma(uint8_t value);
    void setLatch(uint8_t value);

    const PPUControl& getPPUControl();
    const PPUMask& getPPUMask();
    const PPUStatus& getPPUStatus();
    uint8_t getPPUData();
    uint8_t getOamAddr();
    uint8_t getOamData();
    uint8_t getLatch() const;

private:
    uint16_t readNametable();

private:
    std::shared_ptr<CPU> _cpu;
    std::shared_ptr<PPUMemory> _memory;

    PPURegistersAccessor _registers;
    OamDmaAccessor _oamDma;

    PPUControl _ppuControl;
    PPUMask _ppuMask;
    PPUStatus _ppuStatus;
    PPUScroll _ppuScroll;
    PPUAddress _ppuAddress;
    uint8_t _oamAddr;
    uint8_t _oam[0x100];
    uint8_t _latch;
    uint8_t _vramReadBuffer;

    Renderer _renderer;
    int _lineX;
    int _lineY;
    uint16_t _vram;

    master_cycle _masterClock;
    ppu_cycle _ppuClock;
};

}

#endif //NESCORE_PPU_H
