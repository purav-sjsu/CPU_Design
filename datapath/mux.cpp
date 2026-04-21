#include "mux.h"

std::vector<bool> mux2(const std::vector<bool>& a,
                        const std::vector<bool>& b,
                        bool sel) {
    return sel ? b : a;
}

std::vector<bool> mux4(const std::vector<bool>& a,
                        const std::vector<bool>& b,
                        const std::vector<bool>& c,
                        const std::vector<bool>& d,
                        uint8_t sel) {
    switch (sel) {
        case 0:  return a;
        case 1:  return b;
        case 2:  return c;
        default: return d;
    }
}
