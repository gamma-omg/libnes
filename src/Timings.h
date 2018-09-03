#ifndef NESCORE_TIMINGS_H
#define NESCORE_TIMINGS_H

#include <chrono>

namespace nescore
{

using namespace std::chrono;

using master_cycle = duration<uint64_t, std::ratio<1, 21477272>>;
using ppu_cycle = duration<uint64_t, std::ratio_multiply<std::ratio<4>, master_cycle::period>>;
using cpu_cycle = duration<uint64_t, std::ratio_multiply<std::ratio<12>, master_cycle::period>>;

}

#endif //NESCORE_TIMINGS_H
