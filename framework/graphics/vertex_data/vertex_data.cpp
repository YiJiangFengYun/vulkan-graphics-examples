#include "graphics/vertex_data/vertex_data.hpp"

#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/module.hpp"

namespace vg 
{
    VertexData::VertexData()
        : Base(BaseType::VERTEX_DATA)
        , m_subDatas()
        , m_subDataCount()
        , m_bufferSize(0u)
        , m_pBuffer()
        , m_bufferMemorySize()
        , m_pBufferMemory()
        , m_memorySize()
        , m_pMemory(nullptr)
    {

    }

    VertexData::~VertexData()
    {
        if (m_pMemory != nullptr) {
            free(m_pMemory);
        }
    }
    
    uint32_t VertexData::getSubVertexDataCount() const
    {
        return m_subDataCount;
    }
    
    const std::vector<VertexData::SubVertexData> VertexData::getSubVertexDatas() const
    {
        return m_subDatas;
    }

    uint32_t VertexData::getBuffersize() const
    {
        return m_bufferSize;
    }

    std::shared_ptr<vk::Buffer> VertexData::getBuffer() const
    {
        return m_pBuffer;
    }

    uint32_t VertexData::getBufferMemorySize() const
    {
        return m_bufferMemorySize;
    }

    std::shared_ptr<vk::DeviceMemory> VertexData::getBufferMemory() const
    {
        return m_pBufferMemory;
    }

    uint32_t VertexData::getMemorySize() const
    {
        return m_memorySize;
    }

    const void *VertexData::getMemory() const
    {
        return m_pMemory;
    }

    void VertexData::init(uint32_t vertexCount
        , const void *memory
        , uint32_t size
        , Bool32 cacheMemory
        , const vk::PipelineVertexInputStateCreateInfo &vertexInputStateCreateInfo
        )
    {
        std::vector<SubVertexData> subDatas(1);
        SubVertexData &subData = subDatas[0];
        //copy arguments.
        uint32_t count = vertexInputStateCreateInfo.vertexBindingDescriptionCount;
        size_t size = count * sizeof(vk::VertexInputBindingDescription);
        subData.bindingDescs.resize(count);
        memcpy(subData.bindingDescs.data(), vertexInputStateCreateInfo.pVertexBindingDescriptions, size);

        count = vertexInputStateCreateInfo.vertexAttributeDescriptionCount;
        size = count * sizeof(vk::VertexInputAttributeDescription);
        subData.attrDescs.resize(count);
        memcpy(subData.attrDescs.data(), vertexInputStateCreateInfo.pVertexAttributeDescriptions, size);

        //set vertex input state create info.
        subData.vertexInputStateCreateInfo = vertexInputStateCreateInfo;
        subData.vertexInputStateCreateInfo.pNext = nullptr;
        subData.vertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexInputStateCreateInfo.vertexBindingDescriptionCount;
        subData.vertexInputStateCreateInfo.pVertexBindingDescriptions = subData.bindingDescs.data();
        subData.vertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexInputStateCreateInfo.vertexAttributeDescriptionCount;
        subData.vertexInputStateCreateInfo.pVertexAttributeDescriptions = subData.attrDescs.data();

        init(subDatas, memory, size, cacheMemory);
    }

    void VertexData::init(const std::vector<VertexData::SubVertexData> subDatas
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            )
    {
        m_subDataCount = static_cast<uint32_t>(subDatas.size());
        m_subDatas = subDatas;
        
        if (m_pMemory != nullptr && (m_memorySize != size || ! cacheMemory)) {
            free(m_pMemory);
            m_memorySize = 0;
        }
        if (cacheMemory) {
            if (m_pMemory == nullptr) {
                m_pMemory = malloc(size);
                m_memorySize = size;                
            }                      
            memcpy(m_pMemory, memory, size);
        }

        _createBuffer(memory, size);
    }

    void VertexData::_createBuffer(const void *pMemory, uint32_t memorySize)
    {
		auto bufferSize = memorySize;
		//create staging buffer.
		vk::BufferCreateInfo createInfo = {
			vk::BufferCreateFlags(),
			bufferSize,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::SharingMode::eExclusive
		};

		auto pPhysicalDevice = pApp->getPhysicalDevice();
		auto pDevice = pApp->getDevice();
		auto pStagingBuffer = fd::createBuffer(pDevice, createInfo);

		vk::MemoryRequirements memReqs = pDevice->getBufferMemoryRequirements(*pStagingBuffer);
		vk::MemoryAllocateInfo allocateInfo = {
			memReqs.size,
			vg::findMemoryType(pPhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
		};

		auto pStagingBufferMemory = fd::allocateMemory(pDevice, allocateInfo);

		pDevice->bindBufferMemory(*pStagingBuffer, *pStagingBufferMemory, 0u);

		void* data;
		pDevice->mapMemory(*pStagingBufferMemory, 0u, static_cast<vk::DeviceSize>(bufferSize), vk::MemoryMapFlags(), &data);
		uint32_t offset = 0u;
        memcpy(data, pMemory, bufferSize);
		pDevice->unmapMemory(*pStagingBufferMemory);

		//create vertex buffer
        // if old buffer size is same as required buffer size, we don't to create a new buffer for it.
        if (m_bufferSize != bufferSize) {
            m_bufferSize = bufferSize;
		    createInfo.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
		    m_pBuffer = fd::createBuffer(pDevice, createInfo);
		    memReqs = pDevice->getBufferMemoryRequirements(*m_pBuffer);
            m_bufferMemorySize = static_cast<uint32_t>(memReqs.size);        
		    allocateInfo.allocationSize = memReqs.size;
		    allocateInfo.memoryTypeIndex = vg::findMemoryType(pPhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
		    m_pBufferMemory = fd::allocateMemory(pDevice, allocateInfo);
		    pDevice->bindBufferMemory(*m_pBuffer, *m_pBufferMemory, 0u);
        }

		//copy buffer from staging buffer to vertex buffer.
		auto pCommandBuffer = beginSingleTimeCommands();

		vk::BufferCopy copyRegin = {};
		copyRegin.srcOffset = 0;
		copyRegin.dstOffset = 0;
		copyRegin.size = bufferSize;
		pCommandBuffer->copyBuffer(*pStagingBuffer, *m_pBuffer, copyRegin);

		endSingleTimeCommands(pCommandBuffer);
    }
}