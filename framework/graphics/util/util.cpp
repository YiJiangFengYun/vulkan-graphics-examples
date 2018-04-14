#include "graphics/util/util.hpp"

namespace vg {
    uint32_t getNextCapacity(uint32_t current)
    {
        if (current == 0u) return 1u;
        else return static_cast<uint32_t>(std::pow(2, std::log2(current) + 1));
    }
} // vg 