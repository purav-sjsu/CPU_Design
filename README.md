# CPU Design

Goals: Design and implement a complete software CPU in C/C++, including its architecture, ISA, emulator, assembler, and demo programs.

> Design Assumptions
> - All components assume **16-bit** unless configured otherwise
> - All binary values are represented by `std::vector<bool>` with **index 0 as MSB** (most significant bit) and **last index as LSB** (Least significant bit)

## Project Structure

- **gates/** - AND, OR, NOT, XOR, NAND
- **datapath/** - ALU, adders, bitwise logic ops
- **memory/** - Flip-flops, registers, memory
- **clock/** - Clock cycle management
- **utils/** - Helper functions
- **tests/** -


## Build & Run Tests

Requires C++17.

### Gates

```bash
g++ -std=c++17 -o test_gates tests/test_gates.cpp gates/gates.cpp && ./test_gates
```

### Utils

```bash
g++ -std=c++17 -o test_utils tests/test_utils.cpp utils/utils.cpp && ./test_utils
```

### Datapath

Adders
```bash
g++ -std=c++17 -o test_adders tests/alu/test_adders.cpp gates/gates.cpp datapath/adders.cpp utils/utils.cpp && ./test_adders
```

Bitwise Logic Ops
```bash
g++ -std=c++17 -o test_bitwise tests/alu/test_bitwise.cpp gates/gates.cpp datapath/bitwise.cpp utils/utils.cpp && ./test_bitwise
```

Register File
```bash
g++ -std=c++17 -o test_registers tests/registers/test_registers.cpp datapath/regfile.cpp memory/register.cpp memory/flipflop.cpp gates/gates.cpp utils/utils.cpp && ./test_registers
```

### Clock

```bash
g++ -std=c++17 -o test_clock tests/test_clock.cpp clock/clock.cpp && ./test_clock
```

### Memory

D Flip-Flop
```bash
g++ -std=c++17 -o test_flipflop tests/memory/test_flipflop.cpp memory/flipflop.cpp gates/gates.cpp && ./test_flipflop
```

Register
```bash
g++ -std=c++17 -o test_register tests/memory/test_register.cpp memory/register.cpp memory/flipflop.cpp gates/gates.cpp utils/utils.cpp && ./test_register
```

Memory
```bash
g++ -std=c++17 -o test_memory tests/memory/test_memory.cpp memory/memory.cpp memory/register.cpp memory/flipflop.cpp gates/gates.cpp utils/utils.cpp && ./test_memory
```

## Team Members

- Purav
- Daniel Cai
- Sabari Duraipandian
- Simul Barua
