cmake_minimum_required(VERSION 3.12)
project(nescore)

add_subdirectory(libs/gtest)
add_subdirectory(tests)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES src/cpu/CPU.h src/cpu/CPU.cpp src/cpu/access/ZP.h src/cpu/access/IMM.h src/cpu/access/ACC.h src/cpu/access/ZPX.h
        src/cpu/access/ZPY.h src/cpu/access/ABS.h src/cpu/access/ABS.h src/cpu/access/ABSX.h src/cpu/access/ABSY.h
        src/cpu/access/INDX.h src/cpu/access/INDY.h src/cpu/access/IMPL.h src/rom/INESRom.cpp src/rom/INESRom.h
                 src/mappers/IRomMapper.h src/mappers/NROM.cpp src/mappers/NROM.h src/mappers/MapperFactory.h
        src/ppu/PPU.cpp src/ppu/PPU.h src/memory/accessors/IMemoryAccessor.h src/memory/Memory.cpp src/memory/Memory.h
                 src/memory/accessors/BufferAccessor.cpp src/memory/accessors/BufferAccessor.h src/memory/CPUMemory.cpp
                 src/memory/CPUMemory.h src/memory/accessors/MirrorAccessor.cpp src/memory/accessors/MirrorAccessor.h src/ppu/registers/PPUControl.cpp src/ppu/registers/PPUControl.h src/ppu/registers/PPUMask.cpp src/ppu/registers/PPUMask.h src/ppu/registers/PPUStatus.cpp src/ppu/registers/PPUStatus.h src/ppu/registers/PPUScroll.cpp src/ppu/registers/PPUScroll.h src/ppu/registers/PPUAddress.cpp src/ppu/registers/PPUAddress.h src/ppu/registers/PPURegistersAccessor.cpp src/ppu/registers/PPURegistersAccessor.h src/ppu/registers/OamDmaAccessor.cpp src/ppu/registers/OamDmaAccessor.h src/ppu/PPUMemory.cpp src/ppu/PPUMemory.h src/memory/accessors/RomBankAccessor.cpp src/memory/accessors/RomBankAccessor.h)
add_library(nescore ${SOURCE_FILES})
