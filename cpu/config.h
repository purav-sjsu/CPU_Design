#ifndef CONFIG_H
#define CONFIG_H

// Word width in bits (32-bit MIPS-style)
constexpr unsigned int WORD_SIZE  = 32;
// Number of general-purpose registers (MIPS: $0–$31)
constexpr unsigned int NUM_REGS   = 32;
// Number of addressable words in memory
constexpr unsigned int MEM_WORDS  = 4096;

// Special MIPS register indices
constexpr unsigned int REG_ZERO = 0;   // always reads as 0
constexpr unsigned int REG_AT   = 1;   // assembler temporary
constexpr unsigned int REG_SP   = 29;  // stack pointer
constexpr unsigned int REG_FP   = 30;  // frame pointer
constexpr unsigned int REG_RA   = 31;  // return address (JAL writes here)

// Memory map (word addresses)
constexpr unsigned int TEXT_START  = 0x000;  // program code
constexpr unsigned int TEXT_END    = 0x7FF;
constexpr unsigned int DATA_START  = 0x800;  // static data / strings
constexpr unsigned int DATA_END    = 0xEFF;
constexpr unsigned int STACK_TOP   = 0xFEF;  // SP initialised here; grows downward
constexpr unsigned int MMIO_IN     = 0xFFE;  // read a character from stdin
constexpr unsigned int MMIO_OUT    = 0xFFF;  // write a character to stdout

#endif // CONFIG_H
