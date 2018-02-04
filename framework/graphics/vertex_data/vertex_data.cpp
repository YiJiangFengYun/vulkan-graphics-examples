#include "graphics/vertex_data/vertex_data.hpp"

#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/module.hpp"

namespace vg 
{
    VertexData::VertexData()
        : Base(BaseType::VERTEX_DATA)
        , m_bufferMemoryPropertyFlags()
        , m_subDatas()
        , m_subDataCount()
        , m_bufferSize(0u)
        , m_pBuffer()
        , m_bufferMemorySize()
        , m_pBufferMemory()
        , m_memorySize()
        , m_pMemory(nullptr)
        , m_pipelineStateID()
    {
        //default is device local.
        if (! m_bufferMemoryPropertyFlags) 
        {
            m_bufferMemoryPropertyFlags |= MemoryPropertyFlagBits::DEVICE_LOCAL;
        }
    }

    VertexData::VertexData(MemoryPropertyFlags bufferMemoryPropertyFlags)
        : Base(BaseType::VERTEX_DATA)
        , m_bufferMemoryPropertyFlags(bufferMemoryPropertyFlags)
        , m_subDatas()
        , m_subDataCount()
        , m_bufferSize(0u)
        , m_pBuffer()
        , m_bufferMemorySize()
        , m_pBufferMemory()
        , m_memorySize()
        , m_pMemory(nullptr)
        , m_pipelineStateID()
    {
        //default is device local.
        if (! m_bufferMemoryPropertyFlags) 
        {
            m_bufferMemoryPropertyFlags |= MemoryPropertyFlagBits::DEVICE_LOCAL;
        }
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
    
    const std::vector<VertexData::SubVertexData> &VertexData::getSubVertexDatas() const
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
        if (_isDeviceMemoryLocal() == VG_TRUE)
        {
            return m_pMemory;            
        }
        else
        {
            return m_pMmemoryForHostVisible;
        }
    }

    VertexData::PipelineStateID VertexData::getPipelineStateID() const
    {
        return m_pipelineStateID;
    }

    void VertexData::init(uint32_t vertexCount
        , const void *memory
        , uint32_t size
        , Bool32 cacheMemory
        , const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo
        )
    {
        updateDesData(vertexCount, size, vertexInputStateInfo);
        updateBuffer(memory, size, cacheMemory);        
    }

    void VertexData::init(const std::vector<VertexData::SubVertexData> subDatas
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            )
    {
        
        updateDesData(subDatas);
        updateBuffer(memory, size, cacheMemory);

    }

    void VertexData::updateDesData(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo)
    {
        updateDesData(0u, 0u, vertexInputStateInfo);
    }

    void VertexData::updateDesData(uint32_t vertexCount, uint32_t size, const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo)
    {
        if (m_subDatas.size() != 1u || _isEqual(m_subDatas[0].vertexInputStateInfo, vertexInputStateInfo) == VG_FALSE) {
            std::vector<SubVertexData> subDatas(1);
            SubVertexData &subData = subDatas[0];
            //copy arguments.
            uint32_t count = vertexInputStateInfo.vertexBindingDescriptionCount;
            size_t size2 = count * sizeof(vk::VertexInputBindingDescription);
            subData.bindingDescs.resize(count);
            memcpy(subData.bindingDescs.data(), vertexInputStateInfo.pVertexBindingDescriptions, size2);
    
            count = vertexInputStateInfo.vertexAttributeDescriptionCount;
		    size2 = count * sizeof(vk::VertexInputAttributeDescription);
            subData.attrDescs.resize(count);
            memcpy(subData.attrDescs.data(), vertexInputStateInfo.pVertexAttributeDescriptions, size2);
    
            //set vertex input state create info.
            subData.vertexInputStateInfo = vertexInputStateInfo;
            subData.vertexInputStateInfo.pNext = nullptr;
            // subData.vertexInputStateInfo.vertexBindingDescriptionCount = vertexInputStateInfo.vertexBindingDescriptionCount;
            subData.vertexInputStateInfo.pVertexBindingDescriptions = subData.bindingDescs.data();
            // subData.vertexInputStateInfo.vertexAttributeDescriptionCount = vertexInputStateInfo.vertexAttributeDescriptionCount;
            subData.vertexInputStateInfo.pVertexAttributeDescriptions = subData.attrDescs.data();
            
            subData.vertexCount = vertexCount;
            subData.bufferSize = size;
            updateDesData(subDatas);
        }
    }

    void VertexData::updateDesData(const std::vector<SubVertexData> subDatas)
    {
        if (_isEqual(m_subDatas, subDatas) == VG_FALSE) {
            m_subDataCount = static_cast<uint32_t>(subDatas.size());
            m_subDatas = subDatas;
		    for (auto& subData : m_subDatas) 
		    {
		    	subData.vertexInputStateInfo.pVertexBindingDescriptions = subData.bindingDescs.data();
		    	subData.vertexInputStateInfo.pVertexAttributeDescriptions = subData.attrDescs.data();
		    }

            _updatePipelineStateID();            
        }
    }

    void VertexData::updateVertexCount(fd::ArrayProxy<uint32_t> vertexCounts)
    {
        uint32_t size = vertexCounts.size();
        for(uint32_t i = 0; i < size; ++i)
        {
            m_subDatas[i].vertexCount = *(vertexCounts.data() + i);
        }
    }
    
    void VertexData::updateBufferSize(fd::ArrayProxy<uint32_t> bufferSizes)
    {
        uint32_t size = bufferSizes.size();
        for(uint32_t i = 0; i < size; ++i)
        {
            m_subDatas[i].bufferSize = *(bufferSizes.data() + i);
        }
    }

    void VertexData::updateBuffer(const void *memory, uint32_t size, Bool32 cacheMemory)
    {
        MemorySlice slice;
        slice.offset = 0u;
        slice.size = size;
        slice.pMemory = memory;
        updateBuffer(slice, size, cacheMemory);
    }

    void VertexData::updateBuffer(fd::ArrayProxy<MemorySlice> memories
           , uint32_t size
           , Bool32 cacheMemory
           )
    {
        //Caching memory when memory is device local.
        cacheMemory = cacheMemory && _isDeviceMemoryLocal();
        if (m_pMemory != nullptr && (m_memorySize != size || ! cacheMemory)) {
            free(m_pMemory);
            m_pMemory = nullptr;
            m_memorySize = 0;
        }
        if (cacheMemory) {
            if (m_pMemory == nullptr) {
                m_pMemory = malloc(size);
                m_memorySize = size;                
            }
            uint32_t count = memories.size();
            uint32_t offset = 0;
            uint32_t size = 0;
            for (uint32_t i = 0; i < count; ++i) {
                offset = (*(memories.data() + i)).offset;
                size = (*(memories.data() + i)).size;
                memcpy(((char*)m_pMemory + offset), (*(memories.data() + i)).pMemory, size);
            }                     
        }
        _createBuffer(memories, size);
    }

    Bool32 VertexData::_isDeviceMemoryLocal() const
    {
        return (m_bufferMemoryPropertyFlags & MemoryPropertyFlagBits::DEVICE_LOCAL) == MemoryPropertyFlagBits::DEVICE_LOCAL;
    }

    void VertexData::_createBuffer(fd::ArrayProxy<MemorySlice> memories, uint32_t memorySize)
    {
		auto bufferSize = memorySize;

        if (_isDeviceMemoryLocal() == VG_TRUE)
        {
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
		    	vg::findMemoryType(pPhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible)
		    };
    
		    auto pStagingBufferMemory = fd::allocateMemory(pDevice, allocateInfo);
    
		    pDevice->bindBufferMemory(*pStagingBuffer, *pStagingBufferMemory, 0u);
    
		    void* data;
		    pDevice->mapMemory(*pStagingBufferMemory, 0u, static_cast<vk::DeviceSize>(bufferSize), vk::MemoryMapFlags(), &data);
            uint32_t count = memories.size();
            uint32_t offset = 0;
            uint32_t size = 0;
            std::vector<vk::MappedMemoryRange> ranges(count);
            for (uint32_t i = 0; i < count; ++i) {
                offset = (*(memories.data() + i)).offset;
                size = (*(memories.data() + i)).size;
                memcpy(((char*)data + offset), (*(memories.data() + i)).pMemory, size);
                ranges[i].memory = *pStagingBufferMemory;
                ranges[i].offset = offset;
                ranges[i].size = size;
            }
            pDevice->flushMappedMemoryRanges(ranges);
		    pDevice->unmapMemory(*pStagingBufferMemory);
    
		    //create vertex buffer
            // if old buffer size is same as required buffer size, we don't to create a new buffer for it.
            if (m_bufferSize < bufferSize) {
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
            
            {
		        uint32_t offset = 0u;            
		        //copy buffer from staging buffer to vertex buffer.
		        auto pCommandBuffer = beginSingleTimeCommands();

                std::vector<vk::BufferCopy> regions(count);
                for (uint32_t i = 0; i < count; ++i)
                {
                    regions[i].dstOffset = ranges[i].offset;
                    regions[i].srcOffset = ranges[i].offset;                    
                    regions[i].size = ranges[i].size;
                }

		        pCommandBuffer->copyBuffer(*pStagingBuffer, *m_pBuffer, regions);
    
		        endSingleTimeCommands(pCommandBuffer);
            }
        }
        else
        {
             //create vertex buffer
            // if old buffer size is same as required buffer size, we don't to create a new buffer for it.
            if (m_bufferSize < bufferSize)
            {
                m_bufferSize = bufferSize;
                //create staging buffer.
		        vk::BufferCreateInfo createInfo = {
		        	vk::BufferCreateFlags(),
		        	bufferSize,
		        	vk::BufferUsageFlagBits::eVertexBuffer,
		        	vk::SharingMode::eExclusive
		        };
        
		        auto pPhysicalDevice = pApp->getPhysicalDevice();
		        auto pDevice = pApp->getDevice();
		        m_pBuffer = fd::createBuffer(pDevice, createInfo);
                vk::MemoryRequirements memReqs = pDevice->getBufferMemoryRequirements(*m_pBuffer);
		        vk::MemoryAllocateInfo allocateInfo = {
		        	memReqs.size,
		        	vg::findMemoryType(pPhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible)
		        };
        
		        m_pBufferMemory = fd::allocateMemory(pDevice, allocateInfo);
        
		        pDevice->bindBufferMemory(*m_pBuffer, *m_pBufferMemory, 0u);
        
		        pDevice->mapMemory(*m_pBufferMemory, 0u, static_cast<vk::DeviceSize>(bufferSize), vk::MemoryMapFlags(), &m_pMmemoryForHostVisible);
            }
            uint32_t count = memories.size();
            uint32_t offset = 0;
            uint32_t size = 0;
            std::vector<vk::MappedMemoryRange> ranges(count);
            for (uint32_t i = 0; i < count; ++i) {
                offset = (*(memories.data() + i)).offset;
                size = (*(memories.data() + i)).size;
                memcpy(((char*)m_pMmemoryForHostVisible + offset), (*(memories.data() + i)).pMemory, size);
                ranges[i].memory = *m_pBufferMemory;
                ranges[i].offset = offset;
                ranges[i].size = size;
            }
		    auto pDevice = pApp->getDevice();            
            pDevice->flushMappedMemoryRanges(ranges);
        }
		
    }

    void VertexData::_updatePipelineStateID()
	{
		++m_pipelineStateID;
		if ( m_pipelineStateID == std::numeric_limits<PipelineStateID>::max())
		{
			m_pipelineStateID = 0;
		}
	}

    Bool32 VertexData::_isEqual(const std::vector<VertexData::SubVertexData> &subDatas1, 
        const std::vector<VertexData::SubVertexData> &subDatas2)
    {
        if (subDatas1.size() != subDatas2.size()) return VG_FALSE;

        size_t count = subDatas1.size();

        for (size_t i = 0; i < count; ++i)
        {
            if (subDatas1[i].bufferSize != subDatas2[i].bufferSize) return VG_FALSE;
            if (subDatas1[i].vertexCount != subDatas2[i].vertexCount) return VG_FALSE;
            if (_isEqual(subDatas1[i].vertexInputStateInfo, subDatas2[i].vertexInputStateInfo) == VG_FALSE) return VG_FALSE;
        }
    

        return VG_TRUE;
    }

    Bool32 VertexData::_isEqual(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo1, 
            const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo2)
    {
        if (vertexInputStateInfo1.flags != vertexInputStateInfo2.flags) return VG_FALSE;
        if (vertexInputStateInfo1.vertexBindingDescriptionCount != vertexInputStateInfo2.vertexBindingDescriptionCount) return VG_FALSE;
        if (vertexInputStateInfo1.vertexAttributeDescriptionCount != vertexInputStateInfo2.vertexAttributeDescriptionCount) return VG_FALSE;
        if (vertexInputStateInfo1.pNext != vertexInputStateInfo2.pNext) return VG_FALSE;

        uint32_t count = vertexInputStateInfo1.vertexBindingDescriptionCount;
        for (uint32_t i = 0; i < count; ++i) 
        {
            if (*(vertexInputStateInfo1.pVertexBindingDescriptions + i) != *(vertexInputStateInfo2.pVertexBindingDescriptions + i)) return VG_FALSE;
        }

        count = vertexInputStateInfo1.vertexAttributeDescriptionCount;
        for (uint32_t i = 0; i < count; ++i)
        {
            if (*(vertexInputStateInfo1.pVertexAttributeDescriptions + i) != *(vertexInputStateInfo2.pVertexAttributeDescriptions + i)) return VG_FALSE;
        }

        return VG_TRUE;
    }
}