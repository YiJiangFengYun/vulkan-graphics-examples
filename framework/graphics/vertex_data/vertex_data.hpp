#ifndef VG_VERTEX_DATA_H
#define VG_VERTEX_DATA_H

#include <foundation/foundation.hpp>
#include "graphics/global.hpp"

namespace vg 
{
    class VertexData : public Base
    {
    public:
        const std::vector<vk::VertexInputBindingDescription> &getBindingDescs();
        const std::vector<vk::VertexInputAttributeDescription> &getAttributeDecs();
        const vk::PipelineVertexInputStateCreateInfo &getVertexInputStateCreateInfo();
        const vk::PipelineInputAssemblyStateCreateInfo &getInputAssemblyStateCreateInfo();
        uint32_t getVertexCount() const;
        uint32_t getBuffersize() const;
        std::shared_ptr<vk::Buffer> getBuffer() const;
        std::shared_ptr<vk::DeviceMemory> getBufferMemory() const;
        uint32_t getMemorySize() const;
        const void *getMemory() const;

        VertexData();
        VertexData(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateCreateInfo
            , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateCreateInfo
            );

        ~VertexData();

        void init(uint32_t vertexCount
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            );

        void init(uint32_t vertexCount
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            , const vk::PipelineVertexInputStateCreateInfo &vertexInputStateCreateInfo
            , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateCreateInfo 
            );

        template<typename VertexType>
        void init(uint32_t vertexCount 
            , const void *memory
            , Bool32 cacheMemory
            );
        
        template<typename VertexType>
        void init(uint32_t vertexCount 
            , const void *memory
            , Bool32 cacheMemory
            , const vk::PipelineVertexInputStateCreateInfo &vertexInputStateCreateInfo
            , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateCreateInfo);
        
        template<typename VertexType>
        VertexType getVertex(uint32_t index) const;

        template<typename VertexType>
        std::vector<VertexType> getVertices(uint32_t offset, uint32_t count) const;

    private:        
        std::vector<vk::VertexInputBindingDescription> m_bindingDescs;
        std::vector<vk::VertexInputAttributeDescription> m_attrDescs;
        vk::PipelineVertexInputStateCreateInfo m_vertexInputStateCreateInfo;
        vk::PipelineInputAssemblyStateCreateInfo m_inputAssemblyStateCreateInfo;
        uint32_t m_vertexCount;
        uint32_t m_bufferSize;
        std::shared_ptr<vk::Buffer> m_pBuffer;
        std::shared_ptr<vk::DeviceMemory> m_pBufferMemory;
        uint32_t m_memorySize;
        void* m_pMemory;

        void _setVertexInputStateCreateInfo(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateCreateInfo);
        void _setInputAssemblyStateCreateInfo(const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateCreateInfo);
    };

} //!vg

#include "graphics/vertex_data/vertex_data.inl"

#endif //!VG_VERTEX_DATA_H