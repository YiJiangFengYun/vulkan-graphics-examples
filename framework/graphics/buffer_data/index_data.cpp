#include "graphics/buffer_data/index_data.hpp"

#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/module.hpp"
#include "graphics/buffer_data/util.hpp"

namespace vg
{
    IndexData::SubIndexData::SubIndexData(vk::IndexType indexType
        , uint32_t indexCount
        , uint32_t bufferSize
        , vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo
        , uint32_t vertexDataIndex
        )
        : indexType(indexType)
        , indexCount(indexCount)
        , bufferSize(bufferSize)
        , inputAssemblyStateInfo(inputAssemblyStateInfo)
        , vertexDataIndex()
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
        , m_bufferMemoryPropertyFlags(bufferMemoryPropertyFlags)
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

    const IndexData::SubIndexData *IndexData::getSubIndexDatas() const
    {
        return m_subDatas.data();
    }

    uint32_t IndexData::getBufferSize() const
    {
        return m_bufferSize;
    }

    const vk::Buffer *IndexData::getBuffer() const
    {
        return m_pBuffer.get();
    }

    uint32_t IndexData::getBufferMemorySize() const
    {
        return m_bufferMemorySize;
    }

    const vk::DeviceMemory *IndexData::getBufferMemory() const
    {
        return m_pBufferMemory.get();
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

    void IndexData::init(uint32_t subDataCount
            , const SubIndexData *pSubDatas
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
        )
    {
        updateDesData(subDataCount, pSubDatas);
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

    void IndexData::updateDesData(uint32_t subDataCount, const SubIndexData *pSubDatas)
    {
        if (_isEqual(m_subDataCount,
                m_subDatas.data(),
                subDataCount,
                pSubDatas) == VG_FALSE) {
            m_subDataCount = subDataCount;
            m_subDatas.resize(subDataCount);
            memcpy(m_subDatas.data(), pSubDatas, sizeof(SubIndexData) * subDataCount);
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
            m_subDataCount = static_cast<uint32_t>(size);
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
            m_subDataCount = static_cast<uint32_t>(size);            
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

    void IndexData::updateSubDataCount(uint32_t count)
    {
        m_subDatas.resize(count);
        m_subDataCount = count;      
    }

    void IndexData::updateIndexCount(fd::ArrayProxy<uint32_t> indexCounts, uint32_t count, uint32_t offset)
    {
        for(uint32_t i = 0; i < count; ++i)
        {
            m_subDatas[offset].indexCount = *(indexCounts.data() + i);
            ++offset;
        }
    }
    
    void IndexData::updateBufferSize(fd::ArrayProxy<uint32_t> bufferSizes, uint32_t count, uint32_t offset)
    {
        for (uint32_t i = 0; i < count; ++i)
        {
            m_subDatas[offset].bufferSize = *(bufferSizes.data() + i);
            ++offset;
        }
    }

    void IndexData::updateVertexDataIndex(fd::ArrayProxy<uint32_t> vertexDataIndices, uint32_t count, uint32_t offset)
    {
        for (uint32_t i = 0; i < count; ++i)
        {
            m_subDatas[offset].vertexDataIndex = *(vertexDataIndices.data() + i);
            ++offset;
        }
    }

    void IndexData::updateStateInfo(fd::ArrayProxy<vk::PipelineInputAssemblyStateCreateInfo> stateInfos, uint32_t count, uint32_t offset)
    {
        for(uint32_t i = 0; i < count; ++i)
        {
            m_subDatas[offset].inputAssemblyStateInfo = *(stateInfos.data() + i);
            m_subDatas[offset].inputAssemblyStateInfo.pNext = nullptr;
            ++offset;
        }
        _updatePipelineStateID();
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
        if (m_pMemory != nullptr && (m_memorySize < size || ! cacheMemory)) {
            free(m_pMemory);
            m_pMemory = nullptr;
            m_memorySize = 0;
        }

		if (size)
		{
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
    }

    Bool32 IndexData::_isDeviceMemoryLocal() const
    {
        return (m_bufferMemoryPropertyFlags & MemoryPropertyFlagBits::DEVICE_LOCAL) == MemoryPropertyFlagBits::DEVICE_LOCAL;
    }
    
    void IndexData::_createBuffer(fd::ArrayProxy<MemorySlice> memories, uint32_t memorySize)
    {
        createBufferForBufferData(memories, 
            memorySize, 
            _isDeviceMemoryLocal(), 
            vk::BufferUsageFlagBits::eIndexBuffer,
            m_bufferSize,
            m_pBuffer,
            m_bufferMemorySize,
            m_pBufferMemory,
            &m_pMmemoryForHostVisible);
    }

    void IndexData::_updatePipelineStateID()
	{
		++m_pipelineStateID;
		if ( m_pipelineStateID == std::numeric_limits<PipelineStateID>::max())
		{
			m_pipelineStateID = 1;
		}
	}

    Bool32 IndexData::_isEqual(uint32_t subDataCount1, const SubIndexData *pSubDatas1, 
            uint32_t subDataCount2, const SubIndexData *pSubDatas2)
    {
        if (subDataCount1 != subDataCount2) return VG_FALSE;

        size_t count = subDataCount1;
        for (size_t i = 0; i < count; ++i) {
            const auto subData1 = *(pSubDatas1 + i);
            const auto subData2 = *(pSubDatas2 + i);
            if (subData1.bufferSize != subData2.bufferSize) return VG_FALSE;
            if (subData1.indexCount != subData2.indexCount) return VG_FALSE;
            if (subData1.inputAssemblyStateInfo != subData2.inputAssemblyStateInfo) return VG_FALSE;
            if (subData1.indexType != subData2.indexType) return VG_FALSE;
        }

        return VG_TRUE;
    }
}