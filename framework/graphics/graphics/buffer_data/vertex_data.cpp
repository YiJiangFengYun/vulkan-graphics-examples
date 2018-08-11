#include "graphics/buffer_data/vertex_data.hpp"

#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/app/app.hpp"
#include "graphics/buffer_data/util.hpp"

namespace vg 
{
    VertexData::SubVertexData::SubVertexData(uint32_t vertexCount
        , uint32_t bufferSize
        , const uint32_t *pBindingBufferOffsets
        , vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo)
        : vertexCount(vertexCount)
        , bufferSize(bufferSize)
        , pBindingBufferOffsets(pBindingBufferOffsets)
        , vertexInputStateInfo(vertexInputStateInfo)
    {

    }

    VertexData::_SubVertexData::_SubVertexData()
        : bindingDescs()
        , attrDescs()
    {

    }

    VertexData::VertexData(vk::MemoryPropertyFlags bufferMemoryPropertyFlags)
        : Base(BaseType::VERTEX_DATA)
        , m_bufferData(vk::BufferUsageFlagBits::eVertexBuffer
            , bufferMemoryPropertyFlags ? bufferMemoryPropertyFlags : vk::MemoryPropertyFlagBits::eDeviceLocal)
        , m_subDatas()
        , m_subDataCount()
    {
    }

    VertexData::~VertexData()
    {
    }
    
    uint32_t VertexData::getSubVertexDataCount() const
    {
        return m_subDataCount;
    }
    
    const VertexData::SubVertexData *VertexData::getSubVertexDatas() const
    {
        return m_subDatas.data();
    }

    const BufferData &VertexData::getBufferData() const
    {
        return m_bufferData;
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
        , const uint32_t *pBindingBufferOffsets
        )
    {
        updateDesData(vertexCount, size, vertexInputStateInfo, pBindingBufferOffsets);
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
                auto &subData = *(pSubDatas + i);
                auto vertexInputStateInfo = subData.vertexInputStateInfo;
                bindingDescs.resize(vertexInputStateInfo.vertexBindingDescriptionCount);
                memcpy(bindingDescs.data(), vertexInputStateInfo.pVertexBindingDescriptions,
                    sizeof(vk::VertexInputBindingDescription) * vertexInputStateInfo.vertexBindingDescriptionCount);
                auto &attrDescs = m__subDatas[i].attrDescs;
                attrDescs.resize(vertexInputStateInfo.vertexAttributeDescriptionCount);
                memcpy(attrDescs.data(), vertexInputStateInfo.pVertexAttributeDescriptions,
                    sizeof(vk::VertexInputAttributeDescription) * vertexInputStateInfo.vertexAttributeDescriptionCount);
                
                auto &bindingBufferOffsets = m__subDatas[i].bindingBufferOffsets;
                bindingBufferOffsets.resize(vertexInputStateInfo.vertexBindingDescriptionCount);
                memcpy(bindingBufferOffsets.data(), subData.pBindingBufferOffsets, sizeof(uint32_t) * vertexInputStateInfo.vertexBindingDescriptionCount);

                m_subDatas[i].vertexInputStateInfo.pVertexBindingDescriptions = bindingDescs.data();
                m_subDatas[i].vertexInputStateInfo.pVertexAttributeDescriptions = attrDescs.data();
                m_subDatas[i].pBindingBufferOffsets = bindingBufferOffsets.data();
            }
            
        }
    }

    void VertexData::updateDesData(uint32_t vertexCount, uint32_t bufferSize
        , const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo
        , const uint32_t *pBindingBufferOffsets
    )
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
                auto &bindingBufferOffsets = _subData.bindingBufferOffsets;
                bindingBufferOffsets.resize(vertexInputStateInfo.vertexBindingDescriptionCount);
                memcpy(bindingBufferOffsets.data(), pBindingBufferOffsets, sizeof(uint32_t) * vertexInputStateInfo.vertexBindingDescriptionCount);
                //set vertex input state create info.
                subData.vertexInputStateInfo = vertexInputStateInfo;
                subData.vertexInputStateInfo.pNext = nullptr;
                subData.vertexInputStateInfo.pVertexBindingDescriptions = bindingDescs.data();
                subData.vertexInputStateInfo.pVertexAttributeDescriptions = attrDescs.data();
                subData.pBindingBufferOffsets = _subData.bindingBufferOffsets.data();
                
                isChange = VG_TRUE;            
            }
        }
    }

    void VertexData::updateDesData(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo
        , const uint32_t *pBindingBufferOffsets
        )
    {
        size_t size = m_subDatas.size();
        if (size == 0u) {
            m_subDatas.resize(1u);
            m__subDatas.resize(1u);
            size = 1u;
        };
        updateDesData(m_subDatas[0].vertexCount, m_subDatas[0].bufferSize, vertexInputStateInfo, pBindingBufferOffsets);
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

            m_subDatas[i].pBindingBufferOffsets = m__subDatas[i].bindingBufferOffsets.data();
        }
    }

    void VertexData::updateVertexCount(fd::ArrayProxy<uint32_t> vertexCounts, uint32_t offset)
    {
        uint32_t count = static_cast<uint32_t>(vertexCounts.size());
        for(uint32_t i = 0; i < count; ++i)
        {
            m_subDatas[offset].vertexCount = *(vertexCounts.data() + i);
            ++offset;
        }
    }
    
    void VertexData::updateBufferSize(fd::ArrayProxy<uint32_t> bufferSizes, uint32_t offset)
    {
        uint32_t count = static_cast<uint32_t>(bufferSizes.size());
        for(uint32_t i = 0; i < count; ++i)
        {
            m_subDatas[offset].bufferSize = *(bufferSizes.data() + i);
            ++offset;
        }
    }

    void VertexData::updateStateInfo(fd::ArrayProxy<vk::PipelineVertexInputStateCreateInfo> stateInfos, uint32_t offset)
    {
        uint32_t count = static_cast<uint32_t>(stateInfos.size());
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

            subData.pBindingBufferOffsets = _subData.bindingBufferOffsets.data();

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
        m_bufferData.updateBuffer(memories, size, cacheMemory);
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