#ifndef VG_VERTEX_DATA_H
#define VG_VERTEX_DATA_H

#include <foundation/foundation.hpp>
#include "graphics/global.hpp"
#include "graphics/buffer_data/buffer_data_option.hpp"

namespace vg 
{
    class VertexData : public Base
    {
    public:
        using PipelineStateID = uint32_t;
        struct SubVertexData {
            uint32_t vertexCount;
            uint32_t bufferSize;
            vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo;

            SubVertexData(uint32_t vertexCount = 0u
                , uint32_t bufferSize = 0u
                , vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo = vk::PipelineVertexInputStateCreateInfo());
        };
        uint32_t getSubVertexDataCount() const;
        const SubVertexData *getSubVertexDatas() const;
        uint32_t getBuffersize() const;
        const vk::Buffer *getBuffer() const;
        uint32_t getBufferMemorySize() const;
        const vk::DeviceMemory *getBufferMemory() const;
        uint32_t getMemorySize() const;
        const void *getMemory() const;

        VertexData();
        VertexData(MemoryPropertyFlags bufferMemoryPropertyFlags);

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
            );
        
        template<typename VertexType>
        void init(uint32_t vertexCount 
            , const void *memory
            , Bool32 cacheMemory
            , const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo
            );

        void updateDesData(uint32_t subDataCount, const SubVertexData *pSubDatas);        
        void updateDesData(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo);
        void updateDesData(uint32_t vertexCount, uint32_t size, const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo);
        template<typename VertexType>
        void updateDesData(uint32_t vertexCount, const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo);

        void updateSubDataCount(uint32_t count);
        void updateVertexCount(fd::ArrayProxy<uint32_t> vertexCounts, uint32_t count, uint32_t offset = 0u);
        void updateBufferSize(fd::ArrayProxy<uint32_t> bufferSizes, uint32_t count, uint32_t offset = 0u);
        void updateStateInfo(fd::ArrayProxy<vk::PipelineVertexInputStateCreateInfo> stateInfos, uint32_t count, uint32_t offset = 0u);

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
        
        PipelineStateID getPipelineStateID() const;
    private:
        struct _SubVertexData {
            std::vector<vk::VertexInputBindingDescription> bindingDescs;
            std::vector<vk::VertexInputAttributeDescription> attrDescs;
            _SubVertexData();
        };
        MemoryPropertyFlags m_bufferMemoryPropertyFlags;
        PipelineStateID m_pipelineStateID;
        std::vector<SubVertexData> m_subDatas;
        std::vector<_SubVertexData> m__subDatas;
        uint32_t m_subDataCount;
        uint32_t m_bufferSize;
        std::shared_ptr<vk::Buffer> m_pBuffer;
        uint32_t m_bufferMemorySize;
        std::shared_ptr<vk::DeviceMemory> m_pBufferMemory;
        uint32_t m_memorySize;
        void *m_pMemory;
        void *m_pMmemoryForHostVisible;

        Bool32 _isDeviceMemoryLocal() const;
        void _createBuffer(fd::ArrayProxy<MemorySlice> memories, uint32_t memorySize);
        void _updatePipelineStateID();
        Bool32 _isEqual(uint32_t subDataCount1, const SubVertexData *pSubDatas1, 
            uint32_t subDataCount2, const SubVertexData *pSubDatas2);
        Bool32 _isEqual(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo1, 
            const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo2);
    };

} //!vg

#include "graphics/buffer_data/vertex_data.inl"

#endif //!VG_VERTEX_DATA_H