#include "graphics/buffer_data/buffer_data.hpp"

#include "graphics/buffer_data/util.hpp"

namespace vg
{
    BufferData::BufferData(vk::BufferUsageFlags bufferUsageFlags
		, vk::MemoryPropertyFlags memoryPropertyFlags
	    )
        : m_bufferUsageFlags(bufferUsageFlags)
		, m_memoryPropertyFlags(memoryPropertyFlags)
        , m_size()
        , m_bufferSize()
        , m_pBuffer()
        , m_bufferMemorySize()
        , m_pBufferMemory()
        , m_memorySize(0u)
        , m_pMemory(nullptr)
        , m_pMmemoryForHostVisible(nullptr)
    {

    }

    BufferData::~BufferData()
    {
        if (m_pMemory != nullptr)
        {
            free(m_pMemory);
        }
    }

     void BufferData::init(const void *memory
        , uint32_t size
        , Bool32 cacheMemory
        )
    {
         updateBuffer(memory, size, cacheMemory);
    }

    void BufferData::updateBuffer(const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            )
    {
        MemorySlice slice;
        slice.offset = 0u;
        slice.size = size;
        slice.pMemory = memory;
        updateBuffer(slice, size, cacheMemory);
    }

    void BufferData::updateBuffer(fd::ArrayProxy<MemorySlice> memories
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

        m_size = size;

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

	vk::BufferUsageFlags BufferData::getBufferUsageFlags() const
	{
		return m_bufferUsageFlags;
	}

    vk::MemoryPropertyFlags BufferData::getMemoryPropertyFlags() const
    {
        return m_memoryPropertyFlags;
    }

    uint32_t BufferData::getSize() const
    {
        return m_size;
    }

    uint32_t BufferData::getBufferSize() const
    {
        return m_bufferSize;
    }

    const vk::Buffer *BufferData::getBuffer() const
    {
        return m_pBuffer.get();
    }

    uint32_t BufferData::getBufferMemorySize() const
    {
        return m_bufferMemorySize;
    }

    const vk::DeviceMemory *BufferData::getBufferMemory() const
    {
        return m_pBufferMemory.get();
    }

    uint32_t BufferData::getMemorySize() const
    {
        return m_memorySize;
    }

    const void *BufferData::getMemory() const
    {
        return m_pMemory;
    }

    Bool32 BufferData::_isDeviceMemoryLocal() const
    {
        return (m_memoryPropertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal) == 
            vk::MemoryPropertyFlagBits::eDeviceLocal;
    }
        
    void BufferData::_createBuffer(fd::ArrayProxy<MemorySlice> memories, uint32_t memorySize)
    {
        createBufferForBufferData(memories, 
            memorySize, 
            _isDeviceMemoryLocal(), 
            m_bufferUsageFlags,
            m_memoryPropertyFlags,
            m_bufferSize,
            m_pBuffer,
            m_bufferMemorySize,
            m_pBufferMemory,
            &m_pMmemoryForHostVisible
        );
    }
} //vg