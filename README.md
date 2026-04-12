# CPU Design

Goals: Design and implement a complete software CPU in C/C++, including its architecture, ISA, emulator, assembler, and demo programs.

## Project Structure

```
CPU_Design/
├── gates/          # AND, OR, NOT, XOR
├── datapath/       # ALU, adders, bitwise logic ops
├── utils/          # Helper functions
└── tests/          # Test files
    ├── test_gates.cpp
    ├── test_utils.cpp
    └── alu/
        ├── test_adders.cpp
        └── test_bitwise.cpp
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
```

## Team Members

- Purav
- Daniel Cai
- Sabari Duraipandian
- Simul Barua
