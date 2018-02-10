#ifndef VG_VERTEX_DATA_H
#define VG_VERTEX_DATA_H

#include <foundation/foundation.hpp>
#include "graphics/global.hpp"
#include "graphics/vertex_data/vertex_data_option.hpp"

namespace vg 
{
    class VertexData : public Base
    {
    public:
        using PipelineStateID = uint32_t;
        struct SubVertexData {
            uint32_t vertexCount;
            uint32_t bufferSize;
            std::vector<vk::VertexInputBindingDescription> bindingDescs;
            std::vector<vk::VertexInputAttributeDescription> attrDescs;
            vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo;

            SubVertexData(uint32_t vertexCount = 0u
                , uint32_t bufferSize = 0u
                , vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo = vk::PipelineVertexInputStateCreateInfo());
        };
        uint32_t getSubVertexDataCount() const;
        const std::vector<SubVertexData> &getSubVertexDatas() const;
        uint32_t getBuffersize() const;
        std::shared_ptr<vk::Buffer> getBuffer() const;
        uint32_t getBufferMemorySize() const;
        std::shared_ptr<vk::DeviceMemory> getBufferMemory() const;
        uint32_t getMemorySize() const;
        const void *getMemory() const;

        VertexData();
        VertexData(MemoryPropertyFlags bufferMemoryPropertyFlags);

        ~VertexData();

        void init(uint32_t vertexCount
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo 
            );

        void init(const std::vector<SubVertexData> subDatas
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            );
        
        template<typename VertexType>
        void init(uint32_t vertexCount 
            , const void *memory
            , Bool32 cacheMemory
            , const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo
            );

        void updateDesData(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo);
        void updateDesData(uint32_t vertexCount, uint32_t size, const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo);

        void updateDesData(const std::vector<SubVertexData> subDatas);

        template<typename VertexType>
        void updateDesData(uint32_t vertexCount, const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo);

        void updateVertexCount(fd::ArrayProxy<uint32_t> vertexCounts, uint32_t count, uint32_t offset = 0u);
        void updateBufferSize(fd::ArrayProxy<uint32_t> bufferSizes, uint32_t count, uint32_t offset = 0u);

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
        MemoryPropertyFlags m_bufferMemoryPropertyFlags;
        PipelineStateID m_pipelineStateID;
        std::vector<SubVertexData> m_subDatas;
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
        Bool32 _isEqual(const std::vector<VertexData::SubVertexData> &subDatas1, 
            const std::vector<VertexData::SubVertexData> &subDatas2);
        Bool32 _isEqual(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo1, 
            const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo2);
    };

} //!vg

#include "graphics/vertex_data/vertex_data.inl"

#endif //!VG_VERTEX_DATA_H