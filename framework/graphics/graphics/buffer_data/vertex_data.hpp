#ifndef VG_VERTEX_DATA_H
#define VG_VERTEX_DATA_H

#include <foundation/foundation.hpp>
#include "graphics/global.hpp"
#include "graphics/buffer_data/buffer_data_option.hpp"
#include "graphics/buffer_data/buffer_data.hpp"

namespace vg 
{
    class VertexData : public Base
    {
    public:
        struct SubVertexData {
            uint32_t vertexCount;
            uint32_t bufferSize;
            const uint32_t *pBindingBufferOffsets;
            vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo;

            SubVertexData(uint32_t vertexCount = 0u
                , uint32_t bufferSize = 0u
                , const uint32_t *pBindingBufferOffsets = nullptr
                , vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo = vk::PipelineVertexInputStateCreateInfo());
        };
        

        VertexData(vk::MemoryPropertyFlags bufferMemoryPropertyFlags = vk::MemoryPropertyFlags());

        ~VertexData();

        void init(uint32_t subDataCount, 
            const SubVertexData *pSubDatas
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            );

        void init(uint32_t vertexCount
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo
            , const uint32_t *pBindingBufferOffsets
            );

        void updateDesData(uint32_t subDataCount, const SubVertexData *pSubDatas);        
        void updateDesData(uint32_t vertexCount, uint32_t bufferSize, const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo
            , const uint32_t *pBindingBufferOffsets
        );
        void updateDesData(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo
            , const uint32_t *pBindingBufferOffsets
        );

        void updateSubDataCount(uint32_t count);
        void updateVertexCount(fd::ArrayProxy<uint32_t> vertexCounts, uint32_t offset = 0u);
        void updateBufferSize(fd::ArrayProxy<uint32_t> bufferSizes, uint32_t offset = 0u);
        void updateStateInfo(fd::ArrayProxy<vk::PipelineVertexInputStateCreateInfo> stateInfos, uint32_t offset = 0u);

        void updateBuffer(const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            );
        
        void updateBuffer(fd::ArrayProxy<MemorySlice> memories
           , uint32_t size
           , Bool32 cacheMemory
           );
    
        template<typename VertexType>
        VertexType getVertex(uint32_t index) const;

        template<typename VertexType>
        std::vector<VertexType> getVertices(uint32_t offset, uint32_t count) const;

        uint32_t getSubVertexDataCount() const;
        const SubVertexData *getSubVertexDatas() const;
        const BufferData &getBufferData() const;
        
    private:
        struct _SubVertexData {
            std::vector<uint32_t> bindingBufferOffsets;
            std::vector<vk::VertexInputBindingDescription> bindingDescs;
            std::vector<vk::VertexInputAttributeDescription> attrDescs;
            _SubVertexData();
        };
        BufferData m_bufferData;
        std::vector<SubVertexData> m_subDatas;
        std::vector<_SubVertexData> m__subDatas;
        uint32_t m_subDataCount;

        Bool32 _isEqual(uint32_t subDataCount1, const SubVertexData *pSubDatas1, 
            uint32_t subDataCount2, const SubVertexData *pSubDatas2);
        Bool32 _isEqual(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo1, 
            const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo2);
    };

} //!vg

#include "graphics/buffer_data/vertex_data.inl"

#endif //!VG_VERTEX_DATA_H