#ifndef VG_VERTEX_DATA_UTIL_H
#define VG_VERTEX_DATA_UTIL_H

#include "graphics/global.hpp"
#include "graphics/vertex_data/vertex_data.hpp"
#include "graphics/vertex_data/index_data.hpp"

namespace vg
{
    extern void vertexDataToCommandBuffer(const VertexData &vertexData, vk::CommandBuffer &commandBuffer, uint32_t subIndex = 0);

    extern void indexDataToCommandBuffer(const IndexData &indexData, 
        vk::CommandBuffer &commandBuffer, 
        uint32_t subIndex = 0, 
        vk::IndexType indexType = vk::IndexType::eUint32
        );
} //!vg

#endif //!VG_VERTEX_DATA_UTIL_H
