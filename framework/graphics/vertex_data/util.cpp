#include "graphics/vertex_data/util.hpp"

namespace vg
{
    void vertexDataToCommandBuffer(const VertexData &vertexData, vk::CommandBuffer &commandBuffer, uint32_t subIndex)
	{
		const auto &subVertexDatas = vertexData.getSubVertexDatas();
        const auto &subVertexData = subVertexDatas[subIndex];
        const auto &bindingDescs = subVertexData.bindingDescs;
        std::vector<vk::Buffer> vertexBuffers(bindingDescs.size());
		std::vector<vk::DeviceSize> offsets(bindingDescs.size());
        uint32_t offset = 0u;        
        for (uint32_t i = 0; i < subIndex; ++i) {
            offset += subVertexDatas[i].bufferSize;
        }

        uint32_t count = static_cast<uint32_t>(bindingDescs.size());
        for (uint32_t i = 0; i < count; ++i) {
            vertexBuffers[i] = *(vertexData.getBuffer());
            offsets[i] = offset;
            offset += bindingDescs[i].stride * subVertexData.vertexCount;
        }
        
		commandBuffer.bindVertexBuffers(0u, vertexBuffers, offsets);
	}

    void indexDataToCommandBuffer(const IndexData &indexData, 
        vk::CommandBuffer &commandBuffer, 
        uint32_t subIndex, 
        vk::IndexType indexType
        )
    {
        uint32_t offset = 0u;
        uint32_t index = 0u;
        auto subIndexDatas = indexData.getSubIndexDatas();
		for (uint32_t i = 0; i < subIndex; ++i)
		{
			offset += subIndexDatas[i].bufferSize;
		}
		commandBuffer.bindIndexBuffer(*(indexData.getBuffer()), static_cast<vk::DeviceSize>(offset), vk::IndexType::eUint32);
    }
}