#ifndef VG_VERTEX_DATA_OPTION_HPP
#define VG_VERTEX_DATA_OPTION_HPP

#include "graphics/global.hpp"

namespace vg
{
  enum class MemoryPropertyFlagBits
  {
    DEVICE_LOCAL = 1,
    HOST_VISIBLE = 2
  };

  using MemoryPropertyFlags = fd::Flags<MemoryPropertyFlagBits, VkMemoryPropertyFlags>;
} //vg

#endif //VG_VERTEX_DATA_OPTION_HPP