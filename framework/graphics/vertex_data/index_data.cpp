#include "graphics/vertex_data/index_data.hpp"

#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/module.hpp"

namespace vg
{
    IndexData::IndexData()
        : Base(BaseType::INDEX_DATA)
        , m_subDatas()
        , m_subDataCount()
        , m_bufferSize()
        , m_pBuffer()
        , m_bufferMemorySize()
        , m_pBufferMemory()
        , m_memorySize()
        , m_pMemory(nullptr)
    {

    }

    IndexData::~IndexData()
    {
        if (m_pMemory != nullptr) {
            free(m_pMemory);
        }
    }

    uint32_t IndexData::getSubIndexDataCount() const
    {
        return m_subDataCount;
    }

    const std::vector<IndexData::SubIndexData> &IndexData::getSubIndexDatas() const
    {
        return m_subDatas;
    }

    uint32_t IndexData::getBufferSize() const
    {
        return m_bufferSize;
    }

    std::shared_ptr<vk::Buffer> IndexData::getBuffer() const
    {
        return m_pBuffer;
    }

    uint32_t IndexData::getBufferMemorySize() const
    {
        return m_bufferMemorySize;
    }

    std::shared_ptr<vk::DeviceMemory> IndexData::getBufferMemory() const
    {
        return m_pBufferMemory;
    }

    uint32_t IndexData::getMemorySize() const
    {
        return m_memorySize;
    }

    const void *IndexData::getMemory() const
    {
        return m_pMemory;
    }

    void IndexData::init(const std::vector<IndexData::SubIndexData> subDatas
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
            if(m_pMemory == nullptr) {
                m_pMemory = malloc(size);
                m_memorySize = size;                
            }                       
            memcpy(m_pMemory, memory, size);
        }

        _createBuffer(memory, size);
    }

    void IndexData::init(uint32_t indexCount
             , const void *memory
             , uint32_t size
             , Bool32 cacheMemory
             , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateCreateInfo
             )
    {
        std::vector<SubIndexData> subDatas(1);
        SubIndexData &subData = subDatas[0];
        subData.inputAssemblyStateCreateInfo = inputAssemblyStateCreateInfo;
        subData.inputAssemblyStateCreateInfo.pNext = nullptr;

        subData.indexCount = indexCount;
        subData.bufferSize = size;
        init(subDatas, memory, size, cacheMemory);
    }
    
    void IndexData::_createBuffer(const void *pMemory, uint32_t memorySize)
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
		    createInfo.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer;
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