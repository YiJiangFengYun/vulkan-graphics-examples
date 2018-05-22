#include "graphics/buffer_data/index_data.hpp"

#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/app/app.hpp"
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

    IndexData::IndexData(vk::MemoryPropertyFlags bufferMemoryPropertyFlags)
        : Base(BaseType::INDEX_DATA)
        , m_subDatas()
        , m_subDataCount()
        , m_bufferData(bufferMemoryPropertyFlags ? vk::MemoryPropertyFlagBits::eDeviceLocal : bufferMemoryPropertyFlags)
    {
    }

    IndexData::~IndexData()
    {
    }

    uint32_t IndexData::getSubIndexDataCount() const
    {
        return m_subDataCount;
    }

    const IndexData::SubIndexData *IndexData::getSubIndexDatas() const
    {
        return m_subDatas.data();
    }

    const BufferData &IndexData::getBufferData() const
    {
        return m_bufferData;
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
    }

    void IndexData::updateSubDataCount(uint32_t count)
    {
        m_subDatas.resize(count);
        m_subDataCount = count;      
    }

    void IndexData::updateIndexCount(fd::ArrayProxy<uint32_t> indexCounts, uint32_t offset)
    {
        uint32_t count = static_cast<uint32_t>(indexCounts.size());
        for(uint32_t i = 0; i < count; ++i)
        {
            m_subDatas[offset].indexCount = *(indexCounts.data() + i);
            ++offset;
        }
    }
    
    void IndexData::updateBufferSize(fd::ArrayProxy<uint32_t> bufferSizes, uint32_t offset)
    {
        uint32_t count = static_cast<uint32_t>(bufferSizes.size());
        for (uint32_t i = 0; i < count; ++i)
        {
            m_subDatas[offset].bufferSize = *(bufferSizes.data() + i);
            ++offset;
        }
    }

    void IndexData::updateVertexDataIndex(fd::ArrayProxy<uint32_t> vertexDataIndices, uint32_t offset)
    {
        uint32_t count = static_cast<uint32_t>(vertexDataIndices.size());
        for (uint32_t i = 0; i < count; ++i)
        {
            m_subDatas[offset].vertexDataIndex = *(vertexDataIndices.data() + i);
            ++offset;
        }
    }

    void IndexData::updateStateInfo(fd::ArrayProxy<vk::PipelineInputAssemblyStateCreateInfo> stateInfos, uint32_t offset)
    {
        uint32_t count = static_cast<uint32_t>(stateInfos.size());
        for(uint32_t i = 0; i < count; ++i)
        {
            m_subDatas[offset].inputAssemblyStateInfo = *(stateInfos.data() + i);
            m_subDatas[offset].inputAssemblyStateInfo.pNext = nullptr;
            ++offset;
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
        m_bufferData.updateBuffer(memories, size, cacheMemory);
    }

    template<vk::IndexType INDEX_TYPE>
    typename IndexData::IndexTypeInfo<INDEX_TYPE>::ValueType IndexData::getIndex(uint32_t index) const
    {
#ifdef DEBUG
        if (m_pMemory == nullptr) throw std::runtime_error("Failed to get index when not chache memory.");
#endif //!DEBUG
        using IndexType = typename IndexTypeInfo<INDEX_TYPE>::ValueType;
        IndexType resultIndex;
        memcpy(&resultIndex, 
            static_cast<IndexType *>(m_pMemory) + index, 
            sizeof(IndexType));

        return resultIndex;
    }

    //template instantiation
    template typename IndexData::IndexTypeInfo<vk::IndexType::eUint16>::ValueType IndexData::getIndex<vk::IndexType::eUint16>(uint32_t index) const;

    template<vk::IndexType INDEX_TYPE>
    std::vector<typename IndexData::IndexTypeInfo<INDEX_TYPE>::ValueType> IndexData::getIndices(uint32_t offset, uint32_t count) const
    {
#ifdef DEBUG
        if (m_pMemory == nullptr) throw std::runtime_error("Failed to get indices when not chache memory.");
#endif //!DEBUG
        using IndexType = typename IndexTypeInfo<INDEX_TYPE>::ValueType;
        std::vector<IndexType> indices(count);
        memcpy(indices.data(), 
            static_cast<IndexType *>(m_pMemory) + offset, 
            sizeof(IndexType) * static_cast<size_t>(count));
        return indices;
    }
    
    //template instantiation
    template std::vector<typename IndexData::IndexTypeInfo<vk::IndexType::eUint32>::ValueType> IndexData::getIndices<vk::IndexType::eUint32>(uint32_t offset, uint32_t count) const;

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