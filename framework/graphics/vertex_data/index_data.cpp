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
        , m_pipelineStateID()
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

    IndexData::PipelineStateID IndexData::getPipelineStateID() const
    {
        return m_pipelineStateID;
    }

    void IndexData::init(const std::vector<IndexData::SubIndexData> subDatas
        , const void *memory
        , uint32_t size
        , Bool32 cacheMemory
        )
    {
        updateDesData(subDatas);
        updateBuffer(memory, size, cacheMemory);
    }

    void IndexData::init(uint32_t indexCount
             , const void *memory
             , uint32_t size
             , Bool32 cacheMemory
             , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo
             )
    {
        updateDesData(indexCount, size, inputAssemblyStateInfo);
        updateBuffer(memory, size, cacheMemory);
    }

    void IndexData::updateDesData(const std::vector<SubIndexData> subDatas)
    {
        if (_isEqual(m_subDatas, subDatas) == VG_FALSE) {
            m_subDataCount = static_cast<uint32_t>(subDatas.size());
            m_subDatas = subDatas;
            _updatePipelineStateID();            
        }
    }

    void IndexData::updateDesData(uint32_t indexCount, uint32_t size, const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo)
    {
        if (m_subDatas.size() != 1u ||
            m_subDatas[0].bufferSize != size || 
            m_subDatas[0].indexCount != indexCount || 
            m_subDatas[0].inputAssemblyStateInfo != inputAssemblyStateInfo
            )
        {
            std::vector<SubIndexData> subDatas(1);
            SubIndexData &subData = subDatas[0];
            subData.inputAssemblyStateInfo = inputAssemblyStateInfo;
            subData.inputAssemblyStateInfo.pNext = nullptr;
    
            subData.indexCount = indexCount;
            subData.bufferSize = size;
            updateDesData(subDatas);
        }
    }

    void IndexData::updateDesData(const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo)
    {
        updateDesData(0u, 0u, inputAssemblyStateInfo);
    }

    void IndexData::updateIndexCount(fd::ArrayProxy<uint32_t> indexCounts)
    {
        uint32_t size = indexCounts.size();
        for(uint32_t i = 0; i < size; ++i)
        {
            m_subDatas[i].indexCount = *(indexCounts.data() + i);
        }
    }
    
    void IndexData::updateBufferSize(fd::ArrayProxy<uint32_t> bufferSizes)
    {
        uint32_t size = bufferSizes.size();
        for(uint32_t i = 0; i < size; ++i)
        {
            m_subDatas[i].bufferSize = *(bufferSizes.data() + i);
        }
    }

    void IndexData::updateBuffer(const void *memory, uint32_t size, Bool32 cacheMemory)
    {
        if (m_pMemory != nullptr && (m_memorySize != size || ! cacheMemory)) {
            free(m_pMemory);
            m_pMemory = nullptr;
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

    void IndexData::_updatePipelineStateID()
	{
		++m_pipelineStateID;
		if ( m_pipelineStateID == std::numeric_limits<PipelineStateID>::max())
		{
			m_pipelineStateID = 0;
		}
	}

    Bool32 IndexData::_isEqual(std::vector<SubIndexData> subDatas1, std::vector<SubIndexData> subDatas2)
    {
        if (subDatas1.size() != subDatas2.size()) return VG_FALSE;

        size_t count = subDatas1.size();
        for (size_t i = 0; i < count; ++i) {
            if (subDatas1[i].bufferSize != subDatas2[i].bufferSize) return VG_FALSE;
            if (subDatas1[i].indexCount != subDatas2[i].indexCount) return VG_FALSE;
            if (subDatas1[i].inputAssemblyStateInfo != subDatas2[i].inputAssemblyStateInfo) return VG_FALSE;
        }

        return VG_TRUE;
    }
}