#ifndef VG_VERTEX_DATA_H
#define VG_VERTEX_DATA_H

#include <foundation/foundation.hpp>
#include "graphics/global.hpp"

namespace vg 
{
    class VertexData : public Base
    {
    public:
        struct SubVertexData {
            std::vector<vk::VertexInputBindingDescription> bindingDescs;
            std::vector<vk::VertexInputAttributeDescription> attrDescs;
            vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo;
            uint32_t vertexCount;
            uint32_t bufferSize;
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
        
        template<typename VertexType>
        VertexType getVertex(uint32_t index) const;

        template<typename VertexType>
        std::vector<VertexType> getVertices(uint32_t offset, uint32_t count) const;

    private:
        std::vector<SubVertexData> m_subDatas;
        uint32_t m_subDataCount;
        uint32_t m_bufferSize;
        std::shared_ptr<vk::Buffer> m_pBuffer;
        uint32_t m_bufferMemorySize;
        std::shared_ptr<vk::DeviceMemory> m_pBufferMemory;
        uint32_t m_memorySize;
        void *m_pMemory;

        void _createBuffer(const void *pMemory, uint32_t memorySize);
    };

} //!vg

#include "graphics/vertex_data/vertex_data.inl"

#endif //!VG_VERTEX_DATA_H