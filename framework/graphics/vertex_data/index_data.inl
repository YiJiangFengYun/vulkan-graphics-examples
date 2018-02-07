namespace vg
{
    template<typename IndexType>
    void IndexData::init(uint32_t indexCount
            , const void *memory
            , Bool32 cacheMemory
            , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo
            , vk::IndexType indexType
            )
    {
        uint32_t size;
        if (indexType == vk::IndexType::eUint16) {
            size = 16u;
        } else if (indexType == vk::IndexType::eUint32) {
            size = 32u;
        } else {
            throw invalid_argument("Unkown Index Type!");
        }
        size = indexCount * size;
        init(indexCount, memory, size, cacheMemory, inputAssemblyStateInfo, indexType);
    }

    template<typename IndexType>
    void IndexData::updateDesData(uint32_t indexCount
        , const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo
         , vk::IndexType indexType
        )
    {
        uint32_t size;
        if (indexType == vk::IndexType::eUint16) {
            size = 16u;
        } else if (indexType == vk::IndexType::eUint32) {
            size = 32u;
        } else {
            throw invalid_argument("Unkown Index Type!");
        }
        size = indexCount * size;
        updateDesData(indexCount, size, inputAssemblyStateInfo, indexType);
    }

    template<typename IndexType>
    void IndexData::updateDesData(const vk::PipelineInputAssemblyStateCreateInfo &inputAssemblyStateInfo
        , vk::IndexType indexType
        )
    {
        updateDesData(inputAssemblyStateInfo, indexType);
    }

    template<typename IndexType>
    IndexType IndexData::getIndex(uint32_t index) const
    {
#ifdef DEBUG
        if (m_pMemory == nullptr) throw std::runtime_error("Failed to get index when not chache memory.");
#endif //!DEBUG
        IndexType resultIndex;
        memcpy(&resultIndex, 
            static_cast<IndexType *>(m_pMemory) + index, 
            sizeof(IndexType));

        return resultIndex;
    }

    template<typename IndexType>
    std::vector<IndexType> IndexData::getIndices(uint32_t offset, uint32_t count) const
    {
#ifdef DEBUG
        if (m_pMemory == nullptr) throw std::runtime_error("Failed to get indices when not chache memory.");
#endif //!DEBUG
        std::vector<IndexType> indices(count);
        memcpy(indices.data(), 
            static_cast<IndexType *>(m_pMemory) + offset, 
            sizeof(IndexType) * static_cast<size_t>(count));
        return indices;
    }
} //!vg