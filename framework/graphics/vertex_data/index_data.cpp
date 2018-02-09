#include "graphics/vertex_data/index_data.hpp"

#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/module.hpp"

namespace vg
{
    IndexData::SubIndexData::SubIndexData(vk::IndexType indexType
        , uint32_t indexCount
        , uint32_t bufferSize
        , vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo
        , Bool32 hasClipRect
        , fd::Rect2D clipRect)
        : indexType(indexType)
        , indexCount(indexCount)
        , bufferSize(bufferSize)
        , inputAssemblyStateInfo(inputAssemblyStateInfo)
        , hasClipRect(hasClipRect)
        , clipRect(clipRect)
    {

    }
    IndexData::IndexData()
        : Base(BaseType::INDEX_DATA)
        , m_bufferMemoryPropertyFlags()
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
         //default is device local.
        if (! m_bufferMemoryPropertyFlags) 
        {
            m_bufferMemoryPropertyFlags |= MemoryPropertyFlagBits::DEVICE_LOCAL;
        }
    }

    IndexData::IndexData(MemoryPropertyFlags bufferMemoryPropertyFlags)
        : Base(BaseType::INDEX_DATA)
        , m_bufferMemoryPropertyFlags()
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
        //default is device local.
        if (! m_bufferMemoryPropertyFlags) 
        {
            m_bufferMemoryPropertyFlags |= MemoryPropertyFlagBits::DEVICE_LOCAL;
        }
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
        if (_isDeviceMemoryLocal() == VG_TRUE)
        {
            return m_pMemory;            
        }
        else
        {
            return m_pMmemoryForHostVisible;
        }
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
             , vk::IndexType indexType    
             , const void *memory
             , uint32_t size
             , Bool32 cacheMemory
             , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo
             )
    {
        updateDesData(indexCount, indexType, size, inputAssemblyStateInfo);
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

    void IndexData::updateDesData(uint32_t indexCount
        , vk::IndexType indexType    
        , uint32_t bufferSize
        , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo
        )
    {
        size_t size = m_subDatas.size();
        if (size == 0u) {
            size = 1u;
            m_subDatas.resize(1u);
        };
        Bool32 isChange = VG_FALSE;
        for (size_t i = 0u; i < size; ++i) 
        {
            SubIndexData &subData = m_subDatas[i];
            if (subData.indexCount != indexCount) 
            {
                subData.indexCount = indexCount;
                isChange = VG_TRUE;                               
            }

            if (subData.indexType != indexType)
            {
                subData.indexType = indexType;
                isChange = VG_TRUE;
            }

            if (subData.bufferSize != bufferSize)
            {
                subData.bufferSize = bufferSize;
                isChange = VG_TRUE;                                                            
            }

            if (m_subDatas[i].inputAssemblyStateInfo !=inputAssemblyStateInfo) {
                subData.inputAssemblyStateInfo = inputAssemblyStateInfo;
                subData.inputAssemblyStateInfo.pNext = nullptr;
                isChange = VG_TRUE;            
            }
        }

        if (isChange) {
            _updatePipelineStateID();
        }
    }

    void IndexData::updateDesData(vk::IndexType indexType
        ,const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo
        )
    {
        size_t size = m_subDatas.size();
        if (size == 0u) {
            size = 1u;
            m_subDatas.resize(1u);
        };
        Bool32 isChange = VG_FALSE;
        for (size_t i = 0u; i < size; ++i) 
        {
            SubIndexData &subData = m_subDatas[i];
            if (subData.indexType != indexType)
            {
                subData.indexType = indexType;
                isChange = VG_TRUE;
            }

            if (m_subDatas[i].inputAssemblyStateInfo !=inputAssemblyStateInfo) {
                subData.inputAssemblyStateInfo = inputAssemblyStateInfo;
                subData.inputAssemblyStateInfo.pNext = nullptr;
                isChange = VG_TRUE;            
            }
        }

        if (isChange) {
            _updatePipelineStateID();
        }
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
        MemorySlice slice;
        slice.offset = 0u;
        slice.size = size;
        slice.pMemory = memory;
        updateBuffer(slice, size, cacheMemory);
    }

    void IndexData::updateBuffer(fd::ArrayProxy<MemorySlice> memories
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
            if(m_pMemory == nullptr) {
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

    Bool32 IndexData::_isDeviceMemoryLocal() const
    {
        return (m_bufferMemoryPropertyFlags & MemoryPropertyFlagBits::DEVICE_LOCAL) == MemoryPropertyFlagBits::DEVICE_LOCAL;
    }
    
    void IndexData::_createBuffer(fd::ArrayProxy<MemorySlice> memories, uint32_t memorySize)
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
    
		    //create index buffer
            // if old buffer size is same as required buffer size, we don't to create a new buffer for it.
            if (m_bufferSize < bufferSize) {
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
             //create index buffer
            // if old buffer size is same as required buffer size, we don't to create a new buffer for it.
            if (m_bufferSize < bufferSize)
            {
                m_bufferSize = bufferSize;
                //create staging buffer.
		        vk::BufferCreateInfo createInfo = {
		        	vk::BufferCreateFlags(),
		        	bufferSize,
		        	vk::BufferUsageFlagBits::eIndexBuffer,
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
            if (subDatas1[i].indexType != subDatas2[i].indexType) return VG_FALSE;
        }

        return VG_TRUE;
    }
}