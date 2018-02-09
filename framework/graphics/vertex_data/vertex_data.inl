namespace vg
{
    template<typename VertexType>
    void VertexData::init(uint32_t vertexCount 
        , const void *memory
        , Bool32 cacheMemory
        , const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo
        )
    {
        uint32_t size = vertexCount * static_cast<uint32_t>(sizeof(VertexType));
        init(vertexCount, memory, size, cacheMemory, vertexInputStateInfo);
    }

    template<typename VertexType>
    void VertexData::updateDesData(uint32_t vertexCount, const vk::PipelineVertexInputStateCreateInfo &vertexInputStateInfo)
    {
        uint32_t size = vertexCount * static_cast<uint32_t>(sizeof(VertexType));
        updateDesData(vertexCount, size, vertexInputStateInfo);
    }

    template<typename VertexType>
    VertexType VertexData::getVertex(uint32_t index) const
    {
#ifdef DEBUG
        if (m_pMemory == nullptr) throw std::runtime_error("Failed to get vertex when not chache memory.");
#endif //!DEBUG
        VertexType vertex;
        memcpy(&vertex, 
            static_cast<VertexType *>(m_pMemory) + index, 
            sizeof(VertexType));
        return vertex;
    }

    template<typename VertexType>
    std::vector<VertexType> VertexData::getVertices(uint32_t offset, uint32_t count) const
    {
#ifdef DEBUG
        if (m_pMemory == nullptr) throw std::runtime_error("Failed to get verties when not chache memory.");
#endif //!DEBUG
        std::vector<VertexType> vertices(count);
        memcpy(vertices.data(), 
            static_cast<VertexType *>(m_pMemory) + offset, 
            sizeof(VertexType) * static_cast<size_t>(count));
        return vertices;
    }
} //!vg