#ifndef VG_VERTEX_DATA_UTIL_H
#define VG_VERTEX_DATA_UTIL_H

#include "graphics/global.hpp"
#include "graphics/vertex_data/vertex_data.hpp"
#include "graphics/vertex_data/index_data.hpp"

namespace vg
{
    extern void vertexDataToCommandBuffer(vk::CommandBuffer &commandBuffer, const std::shared_ptr<VertexData> &pVertexData, uint32_t subIndex = 0);

    extern void indexDataToCommandBuffer(vk::CommandBuffer &commandBuffer, 
        const std::shared_ptr<IndexData> &pIndexData, 
        uint32_t subIndex = 0
        );
} //!vg

#endif //!VG_VERTEX_DATA_UTIL_H
