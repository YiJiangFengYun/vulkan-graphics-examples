#include "graphics/vertex_data/vertex_data.hpp"

namespace vg 
{
    VertexData::VertexData()
        : Base(BaseType::VERTEX_DATA)
        , m_bindingDescs()
        , m_attrDescs()
        , m_vertexInputStateCreateInfo()
        , m_inputAssemblyStateCreateInfo()
        , m_vertexCount(0u)
        , m_bufferSize(0u)
        , m_pBuffer()
        , m_pBufferMemory()
        , m_memorySize()
        , m_pMemory(nullptr)
    {

    }

    VertexData::VertexData(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateCreateInfo
        , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateCreateInfo
        )
        : Base(BaseType::VERTEX_DATA)
        , m_bindingDescs()
        , m_attrDescs()
        , m_vertexInputStateCreateInfo()
        , m_inputAssemblyStateCreateInfo()
        , m_vertexCount(0u)
        , m_bufferSize(0u)
        , m_pBuffer()
        , m_pBufferMemory()
        , m_memorySize()
        , m_pMemory(nullptr)
    {
        _setVertexInputStateCreateInfo(vertexInputStateCreateInfo);
        _setInputAssemblyStateCreateInfo(inputAssemblyStateCreateInfo);
    }

    VertexData::~VertexData()
    {
        if (m_pMemory != nullptr) {
            free(m_pMemory);
        }
    }

    const std::vector<vk::VertexInputBindingDescription> &VertexData::getBindingDescs()
    {
        return m_bindingDescs;
    }
    const std::vector<vk::VertexInputAttributeDescription> &VertexData::getAttributeDecs()
    {
        return m_attrDescs;
    }
    const vk::PipelineVertexInputStateCreateInfo &VertexData::getVertexInputStateCreateInfo()
    {
        return m_vertexInputStateCreateInfo;
    }

    const vk::PipelineInputAssemblyStateCreateInfo &VertexData::getInputAssemblyStateCreateInfo()
    {
        return m_inputAssemblyStateCreateInfo;
    }

    uint32_t VertexData::getVertexCount() const
    {
        return m_vertexCount;
    }

    uint32_t VertexData::getBuffersize() const
    {
        return m_bufferSize;
    }

    std::shared_ptr<vk::Buffer> VertexData::getBuffer() const
    {
        return m_pBuffer;
    }

    std::shared_ptr<vk::DeviceMemory> VertexData::getBufferMemory() const
    {
        return m_pBufferMemory;
    }

    uint32_t VertexData::getMemorySize() const
    {
        return m_memorySize;
    }

    const void *VertexData::getMemory() const
    {
        return m_pMemory;
    }

    void VertexData::init(uint32_t vertexCount
        , const void *memory
        , uint32_t size
        , Bool32 cacheMemory
        )
    {
        m_vertexCount = vertexCount;
        if (m_pMemory != nullptr && (m_memorySize != size || ! cacheMemory)) {
            free(m_pMemory);
        }

        if (cacheMemory) {
            m_pMemory = malloc(size);                        
            memcpy(m_pMemory, memory, size);
        }
    }

    void VertexData::init(uint32_t vertexCount
        , const void *memory
        , uint32_t size
        , Bool32 cacheMemory
        , const vk::PipelineVertexInputStateCreateInfo &vertexInputStateCreateInfo
        , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateCreateInfo 
        )
    {
        init(vertexCount, memory, size, cacheMemory);
        _setVertexInputStateCreateInfo(vertexInputStateCreateInfo);
        _setInputAssemblyStateCreateInfo(inputAssemblyStateCreateInfo);
    }

    void VertexData::_setVertexInputStateCreateInfo(const vk::PipelineVertexInputStateCreateInfo &vertexInputStateCreateInfo)
    {
        //copy arguments.
        uint32_t count = vertexInputStateCreateInfo.vertexBindingDescriptionCount;
        size_t size = count * sizeof(vk::VertexInputBindingDescription);
        m_bindingDescs.resize(count);
        memcpy(m_bindingDescs.data(), vertexInputStateCreateInfo.pVertexBindingDescriptions, size);

        count = vertexInputStateCreateInfo.vertexAttributeDescriptionCount;
        size = count * sizeof(vk::VertexInputAttributeDescription);
        m_attrDescs.resize(count);
        memcpy(m_attrDescs.data(), vertexInputStateCreateInfo.pVertexAttributeDescriptions, size);

        //set vertex input state create info.
        m_vertexInputStateCreateInfo = vertexInputStateCreateInfo;
        m_vertexInputStateCreateInfo.pNext = nullptr;
        m_vertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexInputStateCreateInfo.vertexBindingDescriptionCount;
        m_vertexInputStateCreateInfo.pVertexBindingDescriptions = m_bindingDescs.data();
        m_vertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexInputStateCreateInfo.vertexAttributeDescriptionCount;
        m_vertexInputStateCreateInfo.pVertexAttributeDescriptions = m_attrDescs.data();
    }

    void VertexData::_setInputAssemblyStateCreateInfo(const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateCreateInfo)
    {
        m_inputAssemblyStateCreateInfo = inputAssemblyStateCreateInfo;
        m_inputAssemblyStateCreateInfo.pNext = nullptr; 
    }
}