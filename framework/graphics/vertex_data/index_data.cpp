#include "graphics/vertex_data/index_data.hpp"

namespace vg
{
    IndexData::IndexData()
        : Base(BaseType::INDEX_DATA)
        , m_indexCount()
        , m_bufferSize()
        , m_pBuffer()
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

    uint32_t IndexData::getIndexCount() const
    {
        return m_indexCount;
    }

    uint32_t IndexData::getBufferSize() const
    {
        return m_bufferSize;
    }

    std::shared_ptr<vk::Buffer> IndexData::getBuffer() const
    {
        return m_pBuffer;
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

    void IndexData::init(uint32_t indexCount
        , const void *memory
        , uint32_t size
        , Bool32 cacheMemory
        )
    {
        m_indexCount = indexCount;
        if (m_pMemory != nullptr && (m_memorySize != size || ! cacheMemory)) {
            free(m_pMemory);
        }

        if (cacheMemory) {
            m_pMemory = malloc(size);                        
            memcpy(m_pMemory, memory, size);
        }
    }
}