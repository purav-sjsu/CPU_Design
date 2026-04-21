#include "sign_extend.h"

std::vector<bool> signExtend(const std::vector<bool>& val, unsigned int targetSize) {
    if (val.size() >= targetSize) return val;
    bool signBit = val[0]; // index 0 = MSB
    std::vector<bool> result(targetSize - val.size(), signBit);
    result.insert(result.end(), val.begin(), val.end());
    return result;
}

std::vector<bool> zeroExtend(const std::vector<bool>& val, unsigned int targetSize) {
    if (val.size() >= targetSize) return val;
    std::vector<bool> result(targetSize - val.size(), false);
    result.insert(result.end(), val.begin(), val.end());
    return result;
}
