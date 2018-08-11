#ifndef VG_BUFFER_DATA_HPP
#define VG_BUFFER_DATA_HPP

#include "graphics/global.hpp"
#include "graphics/buffer_data/buffer_data_option.hpp"

namespace vg {
    class BufferData 
    {
    public:
        BufferData(vk::BufferUsageFlags bufferUsageFlags
            , vk::MemoryPropertyFlags bufferMemoryPropertyFlags);

        ~BufferData();

        void init(const void *memory
            , uint32_t size
            , Bool32 cacheMemory = VG_FALSE
            );

        void updateBuffer(const void *memory
            , uint32_t size
            , Bool32 cacheMemory = VG_FALSE
            );

        void updateBuffer(fd::ArrayProxy<MemorySlice> memories
            , uint32_t size
            , Bool32 cacheMemory = VG_FALSE
            );
        vk::BufferUsageFlags getBufferUsageFlags() const;
        vk::MemoryPropertyFlags getMemoryPropertyFlags() const;
        uint32_t getSize() const;
        uint32_t getBufferSize() const;
        const vk::Buffer *getBuffer() const;
        uint32_t getBufferMemorySize() const;
        const vk::DeviceMemory *getBufferMemory() const;
        uint32_t getMemorySize() const;
        const void *getMemory() const;
    private:
        BufferData() = delete;
        uint32_t m_size;
        uint32_t m_bufferSize;
        vk::BufferUsageFlags m_bufferUsageFlags;
        std::shared_ptr<vk::Buffer> m_pBuffer;
        uint32_t m_bufferMemorySize;
        std::shared_ptr<vk::DeviceMemory> m_pBufferMemory;
        vk::MemoryPropertyFlags m_memoryPropertyFlags;
        uint32_t m_memorySize;
        void *m_pMemory;
        void *m_pMmemoryForHostVisible;

        Bool32 _isDeviceMemoryLocal() const;
        void _createBuffer(fd::ArrayProxy<MemorySlice> memories, uint32_t bufferSize);
    };
} //vg

#endif //VG_BUFFER_DATA_HPP