#ifndef VG_VERTEX_DATA_UTIL_H
#define VG_VERTEX_DATA_UTIL_H

#include "graphics/global.hpp"
#include "graphics/buffer_data/vertex_data.hpp"
#include "graphics/buffer_data/index_data.hpp"

namespace vg
{
    extern void vertexDataToCommandBuffer(vk::CommandBuffer &commandBuffer, const VertexData *pVertexData, uint32_t subIndex = 0);

    extern void indexDataToCommandBuffer(vk::CommandBuffer &commandBuffer, 
        const IndexData *pIndexData, 
        uint32_t subIndex = 0
        );
} //!vg

#endif //!VG_VERTEX_DATA_UTIL_H
