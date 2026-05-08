#include "debug.h"
#include "../cpu/config.h"
#include <climits>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdio>
#include <filesystem>

static const char* REG_NAMES[32] = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0",   "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0",   "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8",   "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};

static std::string formatCycleState(const CPU& cpu) {
    std::ostringstream s;
    unsigned int pc = cpu.getPC();
    uint32_t instr = cpu.getMemoryWord(pc);

    std::string binStr;
    for (int i = 31; i >= 0; --i)
        binStr += ((instr >> i) & 1) ? '1' : '0';

    s << "  {\n";
    s << "    \"cycle\": " << cpu.getCycles() << ",\n";
    s << "    \"pc\": " << pc << ",\n";
    s << "    \"halted\": " << (cpu.isHalted() ? "true" : "false") << ",\n";

    s << "    \"instruction\": {\n";
    s << "      \"decimal\": " << instr << ",\n";
    s << "      \"hex\": \"0x" << std::hex << std::setw(8) << std::setfill('0') << instr << std::dec << "\",\n";
    s << "      \"binary\": \"" << binStr << "\"\n";
    s << "    },\n";

    s << "    \"registers\": {\n";
    for (unsigned int i = 0; i < NUM_REGS; ++i) {
        s << "      \"" << REG_NAMES[i] << "\": " << cpu.getRegister(i);
        if (i + 1 < NUM_REGS) s << ",";
        s << "\n";
    }
    s << "    },\n";

    s << "    \"special\": {\n";
    s << "      \"$sp\": " << cpu.getRegister(REG_SP) << ",\n";
    s << "      \"$fp\": " << cpu.getRegister(REG_FP) << ",\n";
    s << "      \"$ra\": " << cpu.getRegister(REG_RA) << "\n";
    s << "    },\n";

    unsigned int sp = cpu.getRegister(REG_SP);
    unsigned int stack_lo = (sp > 8) ? sp - 8 : 0;
    unsigned int stack_hi = (sp + 8 < MEM_WORDS) ? sp + 8 : MEM_WORDS - 1;

    unsigned int text_lo = (pc > 4) ? pc - 4 : 0;
    unsigned int text_hi = (pc + 4 < TEXT_END) ? pc + 4 : TEXT_END;

    s << "    \"memory\": {\n";

    s << "      \"full\": [";
    for (unsigned int a = 0; a < MEM_WORDS; ++a) {
        s << cpu.getMemoryWord(a);
        if (a + 1 < MEM_WORDS) s << ", ";
    }
    s << "],\n";

    s << "      \"stack\": {";
    for (unsigned int a = stack_lo; a <= stack_hi; ++a) {
        s << "\"" << a << "\": " << cpu.getMemoryWord(a);
        if (a < stack_hi) s << ", ";
    }
    s << "},\n";

    s << "      \"text\": {";
    for (unsigned int a = text_lo; a <= text_hi; ++a) {
        s << "\"" << a << "\": " << cpu.getMemoryWord(a);
        if (a < text_hi) s << ", ";
    }
    s << "}\n";

    s << "    }\n";
    s << "  }";
    return s.str();
}

static void writeDebugLog(const std::vector<std::string>& history) {
    std::ofstream tmp("tmp/debug.json.tmp");
    if (!tmp) return;

    tmp << "[\n";
    for (size_t i = 0; i < history.size(); ++i) {
        tmp << history[i];
        if (i + 1 < history.size()) tmp << ",";
        tmp << "\n";
    }
    tmp << "]\n";
    tmp.close();
    std::rename("tmp/debug.json.tmp", "tmp/debug.json");
}

static void printPrompt(const CPU& cpu) {
    std::cerr << "\n[cycle " << cpu.getCycles()
              << "  PC=0x" << std::hex << std::setw(3) << std::setfill('0') << cpu.getPC() << std::dec << "]\n"
              << "  Enter (step 1 cycle), n <N> (skip N cycles), r (run), q (exit)"
              << ">";
}

void runDebug(CPU& cpu, uint64_t maxCycles) {
    std::filesystem::create_directories("tmp");

    uint64_t limit = (maxCycles == 0) ? UINT64_MAX : maxCycles;
    std::string line;
    std::vector<std::string> history;

    // capture initial state before any steps
    history.push_back(formatCycleState(cpu));
    writeDebugLog(history);

    while (!cpu.isHalted() && cpu.getCycles() < limit) {
        printPrompt(cpu);

        if (!std::getline(std::cin, line)) break;
        std::cerr << "\n";

        if (line == "q") {
            break;
        } else if (line == "r") {
            while (!cpu.isHalted() && cpu.getCycles() < limit) {
                cpu.step();
                history.push_back(formatCycleState(cpu));
                writeDebugLog(history);
            }
            break;
        } else if (line.size() >= 2 && line[0] == 'n' && line[1] == ' ') {
            std::istringstream ss(line.substr(2));
            uint64_t steps = 1;
            ss >> steps;
            for (uint64_t i = 0; i < steps && !cpu.isHalted(); ++i) {
                cpu.step();
                history.push_back(formatCycleState(cpu));
                writeDebugLog(history);
            }
        } else {
            cpu.step();
            history.push_back(formatCycleState(cpu));
            writeDebugLog(history);
        }
    }
}
