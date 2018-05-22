#include "graphics/buffer_data/buffer_data_option.hpp"

namespace vg
{
    MemorySlice::MemorySlice(const void *pMemory
        , uint32_t size
        , uint32_t offset
        )
        : pMemory(pMemory)
        , size(size)
        , offset(offset)
    {

    }
} //vg