cmake_minimum_required(VERSION 3.16)
project(CPU_Design LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Module source files ────────────────────────────────────────────────────────
set(GATES_SRC       gates/gates.cpp)
set(UTILS_SRC       utils/utils.cpp)
set(CLOCK_SRC       clock/clock.cpp)
set(FLIPFLOP_SRC    memory/flipflop.cpp)
set(REGISTER_SRC    memory/register.cpp)
set(MEMORY_SRC      memory/memory.cpp)
set(ADDERS_SRC      datapath/adders.cpp)
set(BITWISE_SRC     datapath/bitwise.cpp)
set(REGFILE_SRC     datapath/regfile.cpp)
set(SHIFTER_SRC     datapath/shifter.cpp)
set(MUX_SRC         datapath/mux.cpp)
set(FLAGS_SRC       datapath/flags.cpp)
set(SIGNEXT_SRC     datapath/sign_extend.cpp)
set(ALU_SRC         datapath/alu.cpp)

set(ISA_SRC         cpu/isa.cpp)
set(BUS_SRC         cpu/bus.cpp)
set(MMIO_SRC        cpu/mmio.cpp)
set(CONTROL_SRC     cpu/control_unit.cpp)
set(CPU_SRC         cpu/cpu.cpp)

set(LEXER_SRC       assembler/lexer.cpp)
set(PARSER_SRC      assembler/parser.cpp)
set(ENCODER_SRC     assembler/encoder.cpp)
set(ASSEMBLER_SRC   assembler/assembler.cpp)

set(LOADER_SRC      emulator/loader.cpp)

# Convenience groupings
set(DATAPATH_ALL
    ${GATES_SRC} ${UTILS_SRC} ${ADDERS_SRC} ${BITWISE_SRC}
    ${REGFILE_SRC} ${MUX_SRC} ${SIGNEXT_SRC} ${ALU_SRC})

set(MEMORY_ALL
    ${FLIPFLOP_SRC} ${REGISTER_SRC} ${MEMORY_SRC})

set(CPU_ALL
    ${DATAPATH_ALL} ${MEMORY_ALL} ${CLOCK_SRC}
    ${ISA_SRC} ${BUS_SRC} ${MMIO_SRC} ${CONTROL_SRC} ${CPU_SRC})

set(ASSEMBLER_ALL
    ${LEXER_SRC} ${PARSER_SRC} ${ENCODER_SRC} ${ASSEMBLER_SRC})

# ── Executables ────────────────────────────────────────────────────────────────
add_executable(assembler assembler/main.cpp ${ASSEMBLER_ALL} ${UTILS_SRC})
add_executable(emulator  emulator/main.cpp  ${LOADER_SRC} ${CPU_ALL} ${UTILS_SRC})

# ── Tests ──────────────────────────────────────────────────────────────────────
enable_testing()

add_executable(test_utils     tests/test_utils.cpp    ${UTILS_SRC})
add_executable(test_gates     tests/test_gates.cpp    ${GATES_SRC})
add_executable(test_clock     tests/test_clock.cpp    ${CLOCK_SRC})
add_executable(test_adders    tests/alu/test_adders.cpp   ${ADDERS_SRC} ${GATES_SRC} ${UTILS_SRC})
add_executable(test_bitwise   tests/alu/test_bitwise.cpp  ${BITWISE_SRC} ${GATES_SRC} ${UTILS_SRC})
add_executable(test_shifter   tests/alu/test_shifter.cpp  ${SHIFTER_SRC} ${GATES_SRC} ${UTILS_SRC})
add_executable(test_mux       tests/alu/test_mux.cpp      ${MUX_SRC} ${GATES_SRC} ${UTILS_SRC})
add_executable(test_flags     tests/alu/test_flags.cpp    ${FLAGS_SRC} ${FLIPFLOP_SRC} ${GATES_SRC} ${UTILS_SRC})
add_executable(test_registers tests/registers/test_registers.cpp
               ${REGFILE_SRC} ${REGISTER_SRC} ${FLIPFLOP_SRC} ${GATES_SRC} ${UTILS_SRC})
add_executable(test_flipflop  tests/memory/test_flipflop.cpp  ${FLIPFLOP_SRC} ${GATES_SRC})
add_executable(test_register  tests/memory/test_register.cpp
               ${REGISTER_SRC} ${FLIPFLOP_SRC} ${GATES_SRC} ${UTILS_SRC})
add_executable(test_memory    tests/memory/test_memory.cpp
               ${MEMORY_SRC} ${REGISTER_SRC} ${FLIPFLOP_SRC} ${GATES_SRC} ${UTILS_SRC})

add_test(NAME utils     COMMAND test_utils)
add_test(NAME gates     COMMAND test_gates)
add_test(NAME clock     COMMAND test_clock)
add_test(NAME adders    COMMAND test_adders)
add_test(NAME bitwise   COMMAND test_bitwise)
add_test(NAME shifter   COMMAND test_shifter)
add_test(NAME mux       COMMAND test_mux)
add_test(NAME flags     COMMAND test_flags)
add_test(NAME registers COMMAND test_registers)
add_test(NAME flipflop  COMMAND test_flipflop)
add_test(NAME register  COMMAND test_register)
add_test(NAME memory    COMMAND test_memory)
