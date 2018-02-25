#include "graphics/vertex_data/util.hpp"

namespace vg
{
    void vertexDataToCommandBuffer(vk::CommandBuffer &commandBuffer, const std::shared_ptr<VertexData> &pVertexData, uint32_t subIndex)
	{
		const auto &subVertexDatas = pVertexData->getSubVertexDatas();
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
            vertexBuffers[i] = *(pVertexData->getBuffer());
            offsets[i] = offset;
            offset += bindingDescs[i].stride * subVertexData.vertexCount;
        }
        
		commandBuffer.bindVertexBuffers(0u, vertexBuffers, offsets);
	}

    void indexDataToCommandBuffer(vk::CommandBuffer &commandBuffer, 
        const std::shared_ptr<IndexData> &pIndexData, 
        uint32_t subIndex
        )
    {
        uint32_t offset = 0u;
        const auto &subIndexDatas = pIndexData->getSubIndexDatas();
		for (uint32_t i = 0; i < subIndex; ++i)
		{
			offset += subIndexDatas[i].bufferSize;
		}
		commandBuffer.bindIndexBuffer(*(pIndexData->getBuffer()), static_cast<vk::DeviceSize>(offset), subIndexDatas[subIndex].indexType);
    }
}