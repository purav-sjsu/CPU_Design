#ifndef DEBUG_H
#define DEBUG_H

#include "../cpu/cpu.h"
#include <cstdint>

void runDebug(CPU& cpu, uint64_t maxCycles);

#endif // DEBUG_H
