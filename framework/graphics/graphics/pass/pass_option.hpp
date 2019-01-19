#ifndef VG_PASS_OPTION_H
#define VG_PASS_OPTION_H

#include <utility>
#include <array>
#include <vulkan/vulkan.hpp>
#include <foundation/util.hpp>
#include "graphics/buffer_data/buffer_data.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/binding_set/binding_set_data.hpp"
namespace vg
{
    using PassDataInfo = BindingSetDataInfo;
    using PassDataSizeInfo = BindingSetDataSizeInfo;

    using PassTextureInfo = BindingSetTextureInfo;
    using PassBufferInfo = BindingSetBufferInfo;
}

#endif // !VG_PASS_OPTION_H
