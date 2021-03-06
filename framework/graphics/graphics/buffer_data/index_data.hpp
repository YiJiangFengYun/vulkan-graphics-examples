#ifndef VG_INDEX_DATA_H
#define VG_INDEX_DATA_H

#include <foundation/foundation.hpp>
#include "graphics/global.hpp"
#include "graphics/buffer_data/buffer_data_option.hpp"
#include "graphics/buffer_data/buffer_data.hpp"

namespace vg {
     class IndexData : public Base
     {
     public:
        template<vk::IndexType indexType>
        struct IndexTypeInfo 
        {
            using ValueType = void;
        };

        template<>
        struct IndexTypeInfo<vk::IndexType::eUint16> 
        {
            using ValueType = uint16_t;
        };

        template<>
        struct IndexTypeInfo<vk::IndexType::eUint32>
        {
            using ValueType = uint32_t;
        };

        struct SubIndexData {
            vk::IndexType indexType;           
            uint32_t indexCount;
            uint32_t bufferSize;
            vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo;
            uint32_t vertexDataIndex;

            SubIndexData(vk::IndexType indexType = vk::IndexType()
                , uint32_t indexCount = 0u
                , uint32_t bufferSize = 0u
                , vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo = vk::PipelineInputAssemblyStateCreateInfo()
                , uint32_t vertexDataIndex = 0u);
        };

        IndexData(vk::MemoryPropertyFlags bufferMemoryPropertyFlags = vk::MemoryPropertyFlags());
        ~IndexData();

        void init(uint32_t subDataCount
            , const SubIndexData *pSubDatas
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            );

        void init(uint32_t indexCount
             , vk::IndexType indexType             
             , const void *memory
             , uint32_t size
             , Bool32 cacheMemory
             , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo
             );

        void updateDesData(uint32_t subDataCount, const SubIndexData *pSubDatas);
        void updateDesData(vk::IndexType indexType, const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo);
        void updateDesData(uint32_t indexCount, vk::IndexType indexType,  uint32_t size, 
            const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo);

        void updateSubDataCount(uint32_t count);
        void updateIndexCount(fd::ArrayProxy<uint32_t> indexCounts, uint32_t offset = 0u);
        void updateBufferSize(fd::ArrayProxy<uint32_t> bufferSizes, uint32_t offset = 0u);
        void updateVertexDataIndex(fd::ArrayProxy<uint32_t> vertexDataIndices, uint32_t offset = 0u);       
        void updateStateInfo(fd::ArrayProxy<vk::PipelineInputAssemblyStateCreateInfo> stateInfos, uint32_t offset = 0u);

        void updateBuffer(const void *memory, uint32_t size, Bool32 cacheMemory);
        void updateBuffer(fd::ArrayProxy<MemorySlice> memories
           , uint32_t size
           , Bool32 cacheMemory
           );

        template<vk::IndexType INDEX_TYPE>
        typename IndexTypeInfo<INDEX_TYPE>::ValueType getIndex(uint32_t index) const;

        template<vk::IndexType INDEX_TYPE>
        std::vector<typename IndexTypeInfo<INDEX_TYPE>::ValueType> getIndices(uint32_t offset, uint32_t count) const;

        uint32_t getSubIndexDataCount() const;
        const SubIndexData *getSubIndexDatas() const;
        const BufferData &getBufferData() const;
        
     private:
        std::vector<SubIndexData> m_subDatas;
        uint32_t m_subDataCount;
        BufferData m_bufferData;
        
        Bool32 _isEqual(uint32_t subDataCount1, const SubIndexData *pSubDatas1, 
            uint32_t subDataCount2, const SubIndexData *pSubDatas2);
     };
} //!vg
#endif //!VG_INDEX_DATA_H