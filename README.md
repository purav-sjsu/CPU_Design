# CPU Design

Goals: Design and implement a complete software CPU in C/C++, including its architecture, ISA, emulator, assembler, and demo programs.

## Project Structure

```
CPU_Design/
├── gates/          # AND, OR, NOT, XOR, NAND
├── datapath/       # ALU, adders, bitwise logic ops
├── memory/         # Flip-flops, registers
├── clock/          # Clock
├── utils/          # Helper functions
└── tests/          # Test files
    ├── test_gates.cpp
    ├── test_utils.cpp
    ├── test_clock.cpp
    ├── alu/
    │   ├── test_adders.cpp
    │   └── test_bitwise.cpp
    └── memory/
        └── test_flipflop.cpp
```

## Design Assumptions

- All binary vectors use `std::vector<bool>` with **index 0 as MSB** (most significant bit)
- All components are **16-bit**

## Build & Run Tests

Requires C++17.

```bash
# Gates
g++ -std=c++17 -o test_gates tests/test_gates.cpp gates/gates.cpp && ./test_gates

# Utils
g++ -std=c++17 -o test_utils tests/test_utils.cpp utils/utils.cpp && ./test_utils

# Adders
g++ -std=c++17 -o test_adders tests/alu/test_adders.cpp gates/gates.cpp datapath/adders.cpp utils/utils.cpp && ./test_adders

# Bitwise
g++ -std=c++17 -o test_bitwise tests/alu/test_bitwise.cpp gates/gates.cpp datapath/bitwise.cpp utils/utils.cpp && ./test_bitwise

# Clock
g++ -std=c++17 -o test_clock tests/test_clock.cpp clock/clock.cpp && ./test_clock

# D Flip-Flop
g++ -std=c++17 -o test_flipflop tests/memory/test_flipflop.cpp memory/flipflop.cpp gates/gates.cpp && ./test_flipflop
```


## Team Members

- Purav
- Daniel Cai
- Sabari Duraipandian
- Simul Barua
