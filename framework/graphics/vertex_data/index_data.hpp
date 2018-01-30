#ifndef VG_INDEX_DATA_H
#define VG_INDEX_DATA_H

#include <foundation/foundation.hpp>
#include "graphics/global.hpp"

namespace vg {
     class IndexData : public Base
     {
     public:
        using PipelineStateID = uint32_t;
        struct SubIndexData {
            vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo;            
            uint32_t indexCount;
            uint32_t bufferSize;
        };
        uint32_t getSubIndexDataCount() const;
        const std::vector<SubIndexData> &getSubIndexDatas() const;
        uint32_t getBufferSize() const;
        std::shared_ptr<vk::Buffer> getBuffer() const;
        uint32_t getBufferMemorySize() const;
        std::shared_ptr<vk::DeviceMemory> getBufferMemory() const;
        uint32_t getMemorySize() const;
        const void *getMemory() const;

        IndexData();
        ~IndexData();

        void init(const std::vector<SubIndexData> subDatas
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            );

        void init(uint32_t indexCount
             , const void *memory
             , uint32_t size
             , Bool32 cacheMemory
             , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo
             );
        
        template<typename IndexType>
        void init(uint32_t indexCount
            , const void *memory
            , Bool32 cacheMemory
            , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo
            );

        template<typename IndexType>
        IndexType getIndex(uint32_t index) const;

        template<typename IndexType>
        std::vector<IndexType> getIndices(uint32_t offset, uint32_t count) const;
        
        PipelineStateID getPipelineStateID() const;
     private:
        PipelineStateID m_pipelineStateID;
        std::vector<SubIndexData> m_subDatas;
        uint32_t m_subDataCount;
        uint32_t m_bufferSize;
        std::shared_ptr<vk::Buffer> m_pBuffer;
        uint32_t m_bufferMemorySize;
        std::shared_ptr<vk::DeviceMemory> m_pBufferMemory;
        uint32_t m_memorySize;
        void *m_pMemory;

        void _createBuffer(const void *pMemory, uint32_t memorySize);
         void _updatePipelineStateID();
     };
} //!vg
#include "graphics/vertex_data/index_data.inl"
#endif //!VG_INDEX_DATA_H