#ifndef MUX_H
#define MUX_H

#include <vector>
#include <cstdint>

// 2-to-1 multiplexer: sel=false → a, sel=true → b
std::vector<bool> mux2(const std::vector<bool>& a,
                        const std::vector<bool>& b,
                        bool sel);

// 4-to-1 multiplexer: sel 0→a, 1→b, 2→c, 3→d
std::vector<bool> mux4(const std::vector<bool>& a,
                        const std::vector<bool>& b,
                        const std::vector<bool>& c,
                        const std::vector<bool>& d,
                        uint8_t sel);

#endif // MUX_H
