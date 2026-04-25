#ifndef BUS_H
#define BUS_H

#include <vector>
#include "../cpu/config.h"

// A shared data/address line between CPU components.
// In a single-cycle design this is just a value latch.
class Bus {
public:
    Bus();
    void              put(const std::vector<bool>& data);
    std::vector<bool> get() const;
    void              reset();
private:
    std::vector<bool> data;
};

#endif // BUS_H
