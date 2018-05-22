#ifndef VG_VERTEX_DATA_OPTION_HPP
#define VG_VERTEX_DATA_OPTION_HPP

#include "graphics/global.hpp"

namespace vg
{
  struct MemorySlice
  {
      const void *pMemory;
      uint32_t size;    
      uint32_t offset;
      MemorySlice(const void *pMemory = nullptr
          , uint32_t size = 0u
          , uint32_t offset = 0u
          );
  };
} //vg

#endif //VG_VERTEX_DATA_OPTION_HPP