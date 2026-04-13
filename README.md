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
- **tests/** - Test files


## Build & Run Tests

Requires C++17 and CMake 3.16+.

Configure the project (generates build files):
```bash
cmake -B build
```

Compile all executables:
```bash
cmake --build build
```

Run all tests:
```bash
ctest --test-dir build --output-on-failure
```

## Team Members

- Purav
- Daniel Cai
- Sabari Duraipandian
- Simul Barua
