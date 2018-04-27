#include "graphics/buffer_data/vertex_data.hpp"

#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/module.hpp"
#include "graphics/buffer_data/util.hpp"

namespace vg 
{
    VertexData::SubVertexData::SubVertexData(uint32_t vertexCount
        , uint32_t bufferSize
        , vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo)
        : vertexCount(vertexCount)
        , bufferSize(bufferSize)
        , vertexInputStateInfo(vertexInputStateInfo)
    {

    }

	VertexData::_SubVertexData::_SubVertexData()
        : bindingDescs()
        , attrDescs()
    {

    }

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
        , m_pMmemoryForHostVisible(nullptr)
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
        , m_pMmemoryForHostVisible(nullptr)
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
    
    const VertexData::SubVertexData *VertexData::getSubVertexDatas() const
    {
        return m_subDatas.data();
    }

    uint32_t VertexData::getBuffersize() const
    {
        return m_bufferSize;
    }

    const vk::Buffer *VertexData::getBuffer() const
    {
        return m_pBuffer.get();
    }

    uint32_t VertexData::getBufferMemorySize() const
    {
        return m_bufferMemorySize;
    }

    const vk::DeviceMemory *VertexData::getBufferMemory() const
    {
        return m_pBufferMemory.get();
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

    void VertexData::init(uint32_t subDataCount, 
            const SubVertexData *pSubDatas
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            )
    {
        
        updateDesData(subDataCount, pSubDatas);
        updateBuffer(memory, size, cacheMemory);
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

    void VertexData::updateDesData(uint32_t subDataCount, const SubVertexData *pSubDatas)
    {
        if (_isEqual(m_subDataCount, m_subDatas.data(), 
                subDataCount, pSubDatas) == VG_FALSE) {
            m_subDataCount = subDataCount;
            m_subDatas.resize(subDataCount);
            memcpy(m_subDatas.data(), pSubDatas, sizeof(SubVertexData) * subDataCount);
            m__subDatas.resize(subDataCount);
            for (uint32_t i = 0; i < subDataCount; ++i)
            {
                auto &bindingDescs = m__subDatas[i].bindingDescs;
                auto vertexInputStateInfo = (*(pSubDatas + i)).vertexInputStateInfo;
				bindingDescs.resize(vertexInputStateInfo.vertexBindingDescriptionCount);
                memcpy(bindingDescs.data(), vertexInputStateInfo.pVertexBindingDescriptions,
                    sizeof(vk::VertexInputBindingDescription) * vertexInputStateInfo.vertexBindingDescriptionCount);
                auto &attrDescs = m__subDatas[i].attrDescs;
                attrDescs.resize(vertexInputStateInfo.vertexAttributeDescriptionCount);
                memcpy(attrDescs.data(), vertexInputStateInfo.pVertexAttributeDescriptions,
                    sizeof(vk::VertexInputAttributeDescription) * vertexInputStateInfo.vertexAttributeDescriptionCount);

                m_subDatas[i].vertexInputStateInfo.pVertexBindingDescriptions = bindingDescs.data();
                m_subDatas[i].vertexInputStateInfo.pVertexAttributeDescriptions = attrDescs.data();

            }
		    
        }
    }

    void VertexData::updateDesData(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo)
    {
        size_t size = m_subDatas.size();
        if (size == 0u) {
			m_subDatas.resize(1u);
			m__subDatas.resize(1u);
			size = 1u;
        };
        Bool32 isChange = VG_FALSE;
        for (size_t i = 0u; i < size; ++i) 
        {
            SubVertexData &subData = m_subDatas[i];
            _SubVertexData &_subData = m__subDatas[i];
            if (_isEqual(subData.vertexInputStateInfo, vertexInputStateInfo) == VG_FALSE) {
                auto &bindingDescs = _subData.bindingDescs;
				bindingDescs.resize(vertexInputStateInfo.vertexBindingDescriptionCount);
                memcpy(bindingDescs.data(), vertexInputStateInfo.pVertexBindingDescriptions,
                    sizeof(vk::VertexInputBindingDescription) * vertexInputStateInfo.vertexBindingDescriptionCount);
                auto &attrDescs = _subData.attrDescs;
                attrDescs.resize(vertexInputStateInfo.vertexAttributeDescriptionCount);
                memcpy(attrDescs.data(), vertexInputStateInfo.pVertexAttributeDescriptions,
                    sizeof(vk::VertexInputAttributeDescription) * vertexInputStateInfo.vertexAttributeDescriptionCount);

                //set vertex input state create info.
                subData.vertexInputStateInfo = vertexInputStateInfo;
                subData.vertexInputStateInfo.pNext = nullptr;
                subData.vertexInputStateInfo.pVertexBindingDescriptions = bindingDescs.data();
                subData.vertexInputStateInfo.pVertexAttributeDescriptions = attrDescs.data();
                
                isChange = VG_TRUE;            
            }
        }
    }

    void VertexData::updateDesData(uint32_t vertexCount, uint32_t bufferSize, const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo)
    {
        size_t size = m_subDatas.size();
        if (size == 0u) {
			m_subDatas.resize(1u);
			m__subDatas.resize(1u);
			size = 1u;
        };
        Bool32 isChange = VG_FALSE;
        for (size_t i = 0u; i < size; ++i) 
        {
            SubVertexData &subData = m_subDatas[i];
            if (subData.vertexCount != vertexCount) 
            {
                subData.vertexCount = vertexCount;
                isChange = VG_TRUE;                               
            }

            if (subData.bufferSize != bufferSize)
            {
                subData.bufferSize = bufferSize;
                isChange = VG_TRUE;                                                            
            }

            _SubVertexData &_subData = m__subDatas[i];
            if (_isEqual(subData.vertexInputStateInfo, vertexInputStateInfo) == VG_FALSE) {
                auto &bindingDescs = _subData.bindingDescs;
				bindingDescs.resize(vertexInputStateInfo.vertexBindingDescriptionCount);
                memcpy(bindingDescs.data(), vertexInputStateInfo.pVertexBindingDescriptions,
                    sizeof(vk::VertexInputBindingDescription) * vertexInputStateInfo.vertexBindingDescriptionCount);
                auto &attrDescs = _subData.attrDescs;
                attrDescs.resize(vertexInputStateInfo.vertexAttributeDescriptionCount);
                memcpy(attrDescs.data(), vertexInputStateInfo.pVertexAttributeDescriptions,
                    sizeof(vk::VertexInputAttributeDescription) * vertexInputStateInfo.vertexAttributeDescriptionCount);

                //set vertex input state create info.
                subData.vertexInputStateInfo = vertexInputStateInfo;
                subData.vertexInputStateInfo.pNext = nullptr;
                subData.vertexInputStateInfo.pVertexBindingDescriptions = bindingDescs.data();
                subData.vertexInputStateInfo.pVertexAttributeDescriptions = attrDescs.data();
                
                isChange = VG_TRUE;            
            }
        }
    }

    void  VertexData::updateSubDataCount(uint32_t count)
    {
        m_subDatas.resize(count);
        m__subDatas.resize(count);
        m_subDataCount = count;
		for (uint32_t i = 0; i < count; ++i)
		{
			auto &inputInfo = m_subDatas[i].vertexInputStateInfo;
			inputInfo.pVertexBindingDescriptions = m__subDatas[i].bindingDescs.data();
			inputInfo.pVertexAttributeDescriptions = m__subDatas[i].attrDescs.data();
		}
    }

    void VertexData::updateVertexCount(fd::ArrayProxy<uint32_t> vertexCounts, uint32_t count, uint32_t offset)
    {
        for(uint32_t i = 0; i < count; ++i)
        {
            m_subDatas[offset].vertexCount = *(vertexCounts.data() + i);
            ++offset;
        }
    }
    
    void VertexData::updateBufferSize(fd::ArrayProxy<uint32_t> bufferSizes, uint32_t count, uint32_t offset)
    {
        for(uint32_t i = 0; i < count; ++i)
        {
            m_subDatas[offset].bufferSize = *(bufferSizes.data() + i);
            ++offset;
        }
    }

    void VertexData::updateStateInfo(fd::ArrayProxy<vk::PipelineVertexInputStateCreateInfo> stateInfos, uint32_t count, uint32_t offset)
    {
        for(uint32_t i = 0; i < count; ++i)
        {
            auto &subData = m_subDatas[offset];
            auto &stateInfo = *(stateInfos.data() + i);
            auto &_subData = m__subDatas[offset];
            auto &bindingDescs = _subData.bindingDescs;
			bindingDescs.resize(stateInfo.vertexBindingDescriptionCount);
            memcpy(bindingDescs.data(), stateInfo.pVertexBindingDescriptions,
                sizeof(vk::VertexInputBindingDescription) * stateInfo.vertexBindingDescriptionCount);
            auto &attrDescs = _subData.attrDescs;
            attrDescs.resize(stateInfo.vertexAttributeDescriptionCount);
            memcpy(attrDescs.data(), stateInfo.pVertexAttributeDescriptions,
                sizeof(vk::VertexInputAttributeDescription) * stateInfo.vertexAttributeDescriptionCount);
            //set vertex input state create info.
            subData.vertexInputStateInfo = stateInfo;
            subData.vertexInputStateInfo.pNext = nullptr;
            subData.vertexInputStateInfo.pVertexBindingDescriptions = bindingDescs.data();
            subData.vertexInputStateInfo.pVertexAttributeDescriptions = attrDescs.data();

            ++offset;
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

    Bool32 VertexData::_isDeviceMemoryLocal() const
    {
        return (m_bufferMemoryPropertyFlags & MemoryPropertyFlagBits::DEVICE_LOCAL) == MemoryPropertyFlagBits::DEVICE_LOCAL;
    }

    void VertexData::_createBuffer(fd::ArrayProxy<MemorySlice> memories, uint32_t memorySize)
    {
		createBufferForBufferData(memories, 
            memorySize, 
            _isDeviceMemoryLocal(), 
            vk::BufferUsageFlagBits::eVertexBuffer,
            m_bufferSize,
            m_pBuffer,
            m_bufferMemorySize,
            m_pBufferMemory,
            &m_pMmemoryForHostVisible);
    }

    Bool32 VertexData::_isEqual(uint32_t subDataCount1, const SubVertexData *pSubDatas1, 
            uint32_t subDataCount2, const SubVertexData *pSubDatas2)
    {
        if (subDataCount1 != subDataCount2) return VG_FALSE;

        size_t count = subDataCount1;

        for (size_t i = 0; i < count; ++i)
        {
            const auto subData1 = *(pSubDatas1 + i);
            const auto subData2 = *(pSubDatas2 + i);
            if (subData1.bufferSize != subData2.bufferSize) return VG_FALSE;
            if (subData1.vertexCount != subData2.vertexCount) return VG_FALSE;
            if (_isEqual(subData1.vertexInputStateInfo, subData2.vertexInputStateInfo) == VG_FALSE) return VG_FALSE;
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