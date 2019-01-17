#include "graphics/binding_set/binding_set.hpp"

#include "graphics/texture/texture_default.hpp"

namespace vg
{
    BindingSet::DataSortInfo::DataSortInfo(std::string name
        , vk::ShaderStageFlags shaderStageFlags
        , uint32_t layoutPriority
        , uint32_t size
        , uint32_t bufferSize
        )
        : name(name)
        , shaderStageFlags(shaderStageFlags)
        , layoutPriority(layoutPriority)
        , size(size)
        , bufferSize(bufferSize)
    {

    }

    Bool32 BindingSet::_compareDataInfo(const DataSortInfo &item1, const DataSortInfo &item2)
    {
        uint32_t shaderStageValue1;
        if ((item1.shaderStageFlags & vk::ShaderStageFlagBits::eAll) == vk::ShaderStageFlagBits::eAll) {
            shaderStageValue1 = -2;
        } else if ((item1.shaderStageFlags & vk::ShaderStageFlagBits::eAllGraphics) == vk::ShaderStageFlagBits::eAllGraphics) {
            shaderStageValue1 = -1;
        } else { 
            shaderStageValue1 = static_cast<uint32_t>(item1.shaderStageFlags);
        }
        uint32_t shaderStageValue2;
        if ((item2.shaderStageFlags & vk::ShaderStageFlagBits::eAll) == vk::ShaderStageFlagBits::eAll) {
            shaderStageValue2 = -2;
        } else if ((item2.shaderStageFlags & vk::ShaderStageFlagBits::eAllGraphics) == vk::ShaderStageFlagBits::eAllGraphics) {
            shaderStageValue2 = -1;
        } else {
            shaderStageValue2 = static_cast<uint32_t>(item2.shaderStageFlags);
        }
        int32_t result = static_cast<int32_t>(shaderStageValue1) - static_cast<int32_t>(shaderStageValue2);
        if (result > 0) {
            return VG_FALSE;
        } else if (result < 0) {
            return VG_TRUE;
        } else {
            return item1.layoutPriority < item2.layoutPriority;
        }
    }

    BindingSet::BufferTextureSortInfo::BufferTextureSortInfo(std::string name
        , uint32_t bindingPriority
        , Bool32 isTexture
        , const void *pData
    )
        : name(name)
        , bindingPriority(bindingPriority)
        , isTexture(isTexture)
        , pData(pData)
    {

    }

    Bool32 BindingSet::_compareBufferTextureInfo(const BufferTextureSortInfo &item1, const BufferTextureSortInfo &item2)
    {
        return item1.bindingPriority < item2.bindingPriority;
    }

    BindingSet::UpdateDescriptorSetInfo::UpdateDescriptorSetInfo(std::vector<vk::DescriptorBufferInfo> bufferInfos
        , std::vector<vk::DescriptorImageInfo> imageInfos
    )
        : bufferInfos(bufferInfos)
        , imageInfos(imageInfos)
    {

    }

    BindingSet::BindingSet() 
        : m_data()
        , m_dataChanged(VG_FALSE)
        , m_dataContentChanged(VG_FALSE)
        , m_dataContentChanges()
        , m_textureChanged(VG_FALSE)
        , m_bufferChanged(VG_FALSE)

        //Applied
        , m_sortDataSet(_compareDataInfo)
        , m_dataBuffer(vk::BufferUsageFlagBits::eUniformBuffer
            , vk::MemoryPropertyFlagBits::eHostVisible)
        , m_sortBufferTexInfosSet(_compareBufferTextureInfo)
        , m_descriptorSetChanged(VG_FALSE)
        , m_layoutBindingCount()
        , m_descriptorSetLayoutBindings()
        , m_updateDescriptorSetInfos()
        , m_pDescriptorSetLayout(nullptr)
        , m_poolSizeInfos()
        , m_pDescriptorPool(nullptr)
        , m_pDescriptorSet(nullptr)
        , m_descriptorSetStateID()
        
    {
    }

    Bool32 BindingSet::hasData(std::string name) const
    {
        return m_data.hasData(name);
    }

    void BindingSet::removeData(std::string name)
    {
        m_data.removeData(name);
        m_dataChanged = VG_TRUE;
    }

    void BindingSet::addData(const std::string name, const BindingSetDataInfo &info, const BindingSetDataSizeInfo &sizeInfo)
    {
        m_data.addData(name, info, sizeInfo);
        m_dataChanged;
    }

    void BindingSet::addData(const std::string name, const BindingSetDataInfo &info, const void *src, uint32_t size)
    {
        m_data.addData(name, info, src, size);
        m_dataChanged = VG_TRUE;
    }
    
    void BindingSet::getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const
    {
        return m_data.getData(name, dst, size, offset);
    }

    void BindingSet::setData(const std::string name, const BindingSetDataInfo &info, const BindingSetDataSizeInfo &sizeInfo)
    {
        m_data.setData(name, info, sizeInfo);
        m_dataChanged = VG_TRUE;
    }
        
    void BindingSet::setData(const std::string name, const void *src, uint32_t size, uint32_t offset)
    {
        m_data.setData(name, src, size, offset);
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[name] = VG_TRUE;
    }

     Bool32 BindingSet::hasBuffer(std::string name) const
    {
        return m_data.hasBuffer(name);
    }

    void BindingSet::addBuffer(std::string name, const BindingSetBufferInfo &bufferInfo)
    {
        m_data.addBuffer(name, bufferInfo);
        m_bufferChanged = VG_TRUE;
    }

    void BindingSet::removeBuffer(std::string name)
    {
        m_data.removeBuffer(name);
        m_bufferChanged = VG_TRUE;
    }

    BindingSetBufferInfo BindingSet::getBuffer(std::string name)
    {
        return m_data.getBufferInfo(name);
    }
        
    void BindingSet::setBuffer(std::string name, const BindingSetBufferInfo &bufferInfo)
    {
        m_data.setBuffer(name, bufferInfo);
        m_bufferChanged = VG_TRUE;
    }

    Bool32 BindingSet::hasTexture(std::string name) const
    {
        return m_data.hasTexture(name);
    }

    void BindingSet::addTexture(std::string name, const BindingSetTextureInfo &texInfo)
    {
        m_data.addTexture(name, texInfo);
        m_textureChanged = VG_TRUE;
    }

    void BindingSet::removeTexture(std::string name)
    {
        m_data.removeTexture(name);
        m_textureChanged = VG_TRUE;
    }

    BindingSetTextureInfo BindingSet::getTexture(std::string name) const
    {
        return m_data.getTextureInfo(name);
    }
    
    void BindingSet::setTexture(std::string name, const BindingSetTextureInfo &texInfo)
    {
        m_data.setTexture(name, texInfo);
        m_textureChanged = VG_TRUE;
    }

    const BufferData &BindingSet::getBufferData() const
    {
        return m_dataBuffer;
    }

    const vk::DescriptorSetLayout *BindingSet::getDescriptorSetLayout() const
    {
        return m_pDescriptorSetLayout.get();
    }

    const vk::DescriptorPool *BindingSet::getDescriptorPool() const
    {
        return m_pDescriptorPool.get();
    }

    const vk::DescriptorSet *BindingSet::getDescriptorSet() const
    {
        return m_pDescriptorSet.get();
    }

    BindingSet::DescriptorSetStateID BindingSet::getDescriptorSetStateID() const
    {
        return m_descriptorSetStateID;
    }

    void BindingSet::apply()
    {
         const auto &data = m_data;
        if (m_dataChanged) {
            //Construct data buffer.
            m_sortDataSet.clear();
            const auto &arrDataNames = data.arrDataNames;
            if (arrDataNames.size() > 0) {
                uint32_t totalBufferSize = 0u;
                for (const auto &name : arrDataNames)
                {
                    const auto &dataInfo = data.getDataInfo(name);
                    const auto &dataSizeInfo = data.getDataSizeInfo(name);
                    DataSortInfo sortInfo = {
                        name,
                        dataInfo.shaderStageFlags,
                        dataInfo.layoutPriority,
                        dataSizeInfo.size,
                        dataSizeInfo.getBufferSize(),
                    };
                    m_sortDataSet.insert(sortInfo);
                    totalBufferSize += sortInfo.bufferSize;
                }

                std::vector<Byte> memory(totalBufferSize);
                uint32_t offset = 0u;
                for (const auto &info : m_sortDataSet) {
                    data.getData(info.name, memory.data() + offset, info.size, 0u);
                    offset += info.bufferSize;
                }
                m_dataBuffer.updateBuffer(memory.data(), totalBufferSize);
            } else {
                m_dataBuffer.updateBuffer(nullptr, 0u);
            }

            //Data change will make build in descriptor set change.
            m_descriptorSetChanged = VG_TRUE;

            m_dataChanged = VG_FALSE;
        }

        if (m_dataContentChanged) {
            //Update data buffer.
            uint32_t count = 0;
            for (const auto &info : m_sortDataSet) {
                if (m_dataContentChanges.count(info.name) != 0) {
                    ++count;
                }
            }
            std::vector<std::vector<Byte>> memories(count);
            std::vector<MemorySlice> memorySlices(count);
            uint32_t index = 0u;
            uint32_t offset = 0u;
            for (const auto &info : m_sortDataSet) {
                if (m_dataContentChanges.count(info.name) != 0) {
                    memories[index].resize(info.bufferSize);
                    data.getData(info.name, memories[index].data(), info.size, 0u);
                    memorySlices[index].pMemory = memories[index].data();
                    memorySlices[index].offset = offset;
                    memorySlices[index].size = info.bufferSize;
                    ++index;
                }
                offset += info.bufferSize;
            }
            m_dataBuffer.updateBuffer(memorySlices, m_dataBuffer.getSize());

            m_dataContentChanged = VG_FALSE;
            m_dataContentChanges.clear();
        }

        if (m_bufferChanged || m_textureChanged) 
        {
            auto &sortInfos = m_sortBufferTexInfosSet;
            sortInfos.clear();
            const auto &arrBufferNames = data.arrBufferNames;
            for (const auto &name : arrBufferNames)
            {
                const auto &bufferData = data.getBufferData(name);
                BufferTextureSortInfo sortInfo = {
                    name,
                    bufferData.bindingPriority,
                    VG_FALSE,
                    reinterpret_cast<const void *>(&bufferData),
                };
                sortInfos.insert(sortInfo);
            }
            const auto &arrTextureNames = data.arrTexNames;
            for (const auto &name : arrTextureNames)
            {
                const auto &textureData = data.getTextureData(name);
                BufferTextureSortInfo sortInfo = {
                    name,
                    textureData.bindingPriority,
                    VG_TRUE,
                    reinterpret_cast<const void *>(&textureData),
                };
                sortInfos.insert(sortInfo);
            }

            //This change will make build in descriptor set change.
            m_descriptorSetChanged = VG_TRUE;

            m_bufferChanged = VG_FALSE;
            m_textureChanged = VG_FALSE;
        }

        if (m_descriptorSetChanged) {
            // m_layoutBindingCount = 0u;
            uint32_t currBinding = 0u;

            //first part descriptors is build data.
            uint32_t dataBindingCount = 0u;
            std::vector<vk::DescriptorSetLayoutBinding> dataBindingInfos;
            std::vector<UpdateDescriptorSetInfo> dataUpdateDesSetInfos;
            {
                vk::ShaderStageFlags currStageFlags = vk::ShaderStageFlags();
                for (const auto &info : m_sortDataSet) {
                    if (info.shaderStageFlags != currStageFlags) {
                        currStageFlags = info.shaderStageFlags;
                        ++dataBindingCount;
                    }
                }
                if (dataBindingCount)
                {
                    dataBindingInfos.resize(dataBindingCount);
                    dataUpdateDesSetInfos.resize(dataBindingCount);
                    currStageFlags = vk::ShaderStageFlags();
                    uint32_t dataBindingIndex = 0u;
                    auto iterator = m_sortDataSet.begin();
                    uint32_t offset = 0u;
                    do {
                        const auto &info = *iterator;
                        if (info.shaderStageFlags != currStageFlags) {
                            currStageFlags = info.shaderStageFlags;
                            
                            vk::DescriptorSetLayoutBinding dataBindingInfo;
                            dataBindingInfo.binding = currBinding;
                            ++currBinding;
                            dataBindingInfo.descriptorType = vk::DescriptorType::eUniformBuffer;
                            dataBindingInfo.descriptorCount = 1u;
                            dataBindingInfo.stageFlags = info.shaderStageFlags;
                            dataBindingInfos[dataBindingIndex] = dataBindingInfo;

                            UpdateDescriptorSetInfo dataUpdateDesSetInfo;
                            dataUpdateDesSetInfo.bufferInfos.resize(1u);
                            dataUpdateDesSetInfo.bufferInfos[0].buffer = *(m_dataBuffer.getBuffer());
                            dataUpdateDesSetInfo.bufferInfos[0].offset = offset;

                            uint32_t range = info.bufferSize;
                            auto nextIterator = iterator;
                            ++nextIterator;
                            while (nextIterator != m_sortDataSet.end() && nextIterator->shaderStageFlags == currStageFlags)
                            {
                                range += nextIterator->bufferSize;
                                ++nextIterator;
                            }

                            dataUpdateDesSetInfo.bufferInfos[0].range = range;
                            dataUpdateDesSetInfos[dataBindingIndex] = dataUpdateDesSetInfo;

                            ++dataBindingIndex;
                        }
                        offset += info.bufferSize;
                        ++iterator;
                    } while (iterator != m_sortDataSet.end());
                }
            }

            //second part is buffer and image bindings.
            uint32_t bufferTextureBindingCount = 0u;
            std::vector<vk::DescriptorSetLayoutBinding> bufferTextureBindingInfos;
            std::vector<UpdateDescriptorSetInfo> bufferTextureUpdateDesSetInfos;
            {
                auto sortInfos = m_sortBufferTexInfosSet;
                //fill infos for creating descriptor set and its layout.
                bufferTextureBindingCount = sortInfos.size();
                bufferTextureBindingInfos.resize(bufferTextureBindingCount);
                bufferTextureUpdateDesSetInfos.resize(bufferTextureBindingCount);
                uint32_t bufferTextureBindingIndex = 0u;
                for (auto iterator = sortInfos.begin(); iterator != sortInfos.end(); ++iterator)
                {
                    const auto &sortInfo = *iterator;
                    vk::DescriptorSetLayoutBinding bindingInfo;
                    UpdateDescriptorSetInfo updateDesSetInfo;
                    bindingInfo.binding = currBinding;
                    ++currBinding;
                    if (sortInfo.isTexture == VG_TRUE)
                    {
                        const auto &textureData = *(reinterpret_cast<const BindingSetTextureData *>(sortInfo.pData));
                        bindingInfo.descriptorType = tranImageDescriptorTypeToVK(textureData.descriptorType);
                        bindingInfo.stageFlags = textureData.stageFlags;
                        auto textureCount = static_cast<uint32_t>(textureData.textures.size());
                        auto pDefaultTexture = getDefaultTexture(tranSamplerTextureTypeToVKImageViewType(textureData.textureType));
                        bindingInfo.descriptorCount = textureCount;
                        updateDesSetInfo.imageInfos.resize(textureCount);
                        for (uint32_t i = 0; i < textureCount; ++i)
                        {
                            auto oneTexInfo = textureData.textures[i];
                            const auto pTexture = oneTexInfo.pTexture != nullptr ? oneTexInfo.pTexture : pDefaultTexture;
                            vk::ImageView imageView;
                            if (oneTexInfo.pTexture != nullptr) {
                                if (oneTexInfo.pImageView != nullptr) {
                                    imageView = *(oneTexInfo.pImageView->getImageView());
                                } else {
                                    imageView = *(pTexture->getImageView()->getImageView());
                                }
                            } else {
                                imageView = *(pTexture->getImageView()->getImageView());
                            }
                            vk::Sampler sampler;
                            if (oneTexInfo.pSampler != nullptr) {
                                sampler = *(oneTexInfo.pSampler->getSampler());
                            } else {
                                sampler = *(pTexture->getSampler()->getSampler());
                            }
                            vk::ImageLayout imageLayout;
                            if (oneTexInfo.imageLayout != vk::ImageLayout::eUndefined) {
                                imageLayout = oneTexInfo.imageLayout;
                            } else {
                                imageLayout = pTexture->getImage()->getInfo().layout;
                            }
                            updateDesSetInfo.imageInfos[i].imageView = imageView;
                            updateDesSetInfo.imageInfos[i].sampler = sampler;
                            updateDesSetInfo.imageInfos[i].imageLayout = imageLayout;
                        }
                    }
                    else
                    {
                        const auto &bufferData = *(reinterpret_cast<const BindingSetBufferData *>(sortInfo.pData));
                        bindingInfo.descriptorType = tranBufferDescriptorTypeToVK(bufferData.descriptorType);
                        bindingInfo.stageFlags = bufferData.stageFlags;
                        auto bufferCount = static_cast<uint32_t>(bufferData.buffers.size());
                        bindingInfo.descriptorCount = bufferCount;
                        updateDesSetInfo.bufferInfos.resize(bufferCount);                        
                        for (uint32_t i = 0; i < bufferCount; ++i)
                        {
                            auto oneBufferInfo = bufferData.buffers[i];
                            updateDesSetInfo.bufferInfos[i].buffer = *(oneBufferInfo.pBuffer->getBuffer());
                            updateDesSetInfo.bufferInfos[i].offset = oneBufferInfo.offset;
                            updateDesSetInfo.bufferInfos[i].range = oneBufferInfo.range;
                        }
                    }
                    bufferTextureBindingInfos[bufferTextureBindingIndex] = bindingInfo;
                    bufferTextureUpdateDesSetInfos[bufferTextureBindingIndex] = updateDesSetInfo;

                    ++bufferTextureBindingIndex;
                }
            }

            //get final total descriptor set info.
            uint32_t layoutBindingCount = 0u;
            std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings;
            std::vector<UpdateDescriptorSetInfo> updateDescriptorSetInfos;
            {
                layoutBindingCount = dataBindingCount + bufferTextureBindingCount;
                descriptorSetLayoutBindings.resize(layoutBindingCount);
                updateDescriptorSetInfos.resize(layoutBindingCount);
                uint32_t i = 0;
                uint32_t index = 0u;
                for (i = 0; i < dataBindingCount; ++i) {
                    descriptorSetLayoutBindings[index] = dataBindingInfos[i];
                    updateDescriptorSetInfos[index] = dataUpdateDesSetInfos[i];
                    ++index;
                }
                for (i = 0; i < bufferTextureBindingCount; ++i) {
                    descriptorSetLayoutBindings[index] = bufferTextureBindingInfos[i];
                    updateDescriptorSetInfos[index] = bufferTextureUpdateDesSetInfos[i];
                    ++index;
                }
            }

            //Create descriptor set layout.
            Bool32 createdDescriptorSetLayout = VG_FALSE;
            {
                if (m_layoutBindingCount != layoutBindingCount || m_descriptorSetLayoutBindings != descriptorSetLayoutBindings)
                {
                    m_layoutBindingCount = layoutBindingCount;
                    m_descriptorSetLayoutBindings = descriptorSetLayoutBindings;
                    auto pDevice = pApp->getDevice();
                    if (layoutBindingCount != 0u)
                    {
                        vk::DescriptorSetLayoutCreateInfo createInfo =
                        {
                            vk::DescriptorSetLayoutCreateFlags(),
                            layoutBindingCount,
                            descriptorSetLayoutBindings.data()
                        };
        
                        m_pDescriptorSetLayout = fd::createDescriptorSetLayout(pDevice, createInfo);
                    }
                    else
                    {
                        m_pDescriptorSetLayout = nullptr;
                    }
                    createdDescriptorSetLayout = VG_TRUE;
                    _updateDescriptorSetStateID();
                    // Create descriptor set layout will make pipeline layout change.
                    // m_pipelineLayoutChanged = VG_TRUE;
                    // Create descriptor set layout will make descriptor sets change.
                    // m_descriptorSetsChanged = VG_TRUE;
                }
            }

            //Create descriptor pool.
            std::shared_ptr<vk::DescriptorPool> pOldPool = nullptr;
            Bool32 createdPool = VG_FALSE;
            {
                if (createdDescriptorSetLayout == VG_TRUE)
                {
                    //Check and reCreate descriptor pool.
                    //Caculate current need pool size info.
                    std::unordered_map<vk::DescriptorType, uint32_t> poolSizeInfos;
                    for (uint32_t i = 0; i < layoutBindingCount; ++i) {
                        const auto &bindingInfo = descriptorSetLayoutBindings[i];
                        poolSizeInfos[bindingInfo.descriptorType] += bindingInfo.descriptorCount;
                    }
        
                    //Check pool size is greater than need. if not, recreating pool.
                    Bool32 isGreater = VG_TRUE;
                    for (const auto &pair : poolSizeInfos) {
                        if (m_poolSizeInfos[pair.first] < poolSizeInfos[pair.first]) {
                            isGreater = VG_FALSE;
                            break;
                        }
                    }
        
                    pOldPool = m_pDescriptorPool;
                    if (isGreater == VG_FALSE)
                    {
                        //create descriptor pool.                
                        std::vector<vk::DescriptorPoolSize> arrPoolSizeInfos(poolSizeInfos.size());
                        size_t index = 0;
                        for (const auto& item : poolSizeInfos)
                        {
                            arrPoolSizeInfos[index].type = item.first;
                            arrPoolSizeInfos[index].descriptorCount = item.second;
                            ++index;
                        }
        
                        auto pDevice = pApp->getDevice();
                        {
                            if (arrPoolSizeInfos.size())
                            {
                                vk::DescriptorPoolCreateInfo createInfo = { vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet
                                    , 1u
                                    , static_cast<uint32_t>(arrPoolSizeInfos.size())
                                    , arrPoolSizeInfos.data()
                                };
                                m_pDescriptorPool = fd::createDescriptorPool(pDevice, createInfo);
                            }
                            else
                            {
                                m_pDescriptorPool = nullptr;
                            }
                        }
        
                        m_poolSizeInfos = poolSizeInfos;
                        createdPool = VG_TRUE;
                    }
                }
            }
            //Reallocte descriptor set.
            {
                if (createdPool == VG_TRUE || createdDescriptorSetLayout == VG_TRUE) {
                    auto pDevice = pApp->getDevice();
                    //create descriptor set.
                    if (m_pDescriptorSetLayout != nullptr && m_pDescriptorPool != nullptr)
                    {
                        vk::DescriptorSetLayout layouts[] = { *m_pDescriptorSetLayout };
                        vk::DescriptorSetAllocateInfo allocateInfo = {
                            *m_pDescriptorPool,
                            1u,
                            layouts
                        };
                        //first we should free old descriptor set because pool may be old pool than is empty and allocated to create this descriptor set.
                        m_pDescriptorSet = nullptr;
                        m_pDescriptorSet = fd::allocateDescriptorSet(pDevice, m_pDescriptorPool.get(), allocateInfo);
                        // m_usingDescriptorSets[0] = *m_pDescriptorSet;
                    }
                    else
                    {
                        m_pDescriptorSet = nullptr;
                    }

                    //Reallocate descriptor set will make descriptor sets change.
                    _updateDescriptorSetStateID();
                    // m_descriptorSetsChanged = VG_TRUE;
                }
            }

            //Update descriptor set
            {
                std::vector<vk::WriteDescriptorSet> writes(layoutBindingCount);
                for (uint32_t i = 0; i < layoutBindingCount; ++i)
                {
                    writes[i].descriptorType = descriptorSetLayoutBindings[i].descriptorType;
                    writes[i].descriptorCount = descriptorSetLayoutBindings[i].descriptorCount;
                    writes[i].dstArrayElement = 0u;
                    writes[i].dstBinding = descriptorSetLayoutBindings[i].binding;
                    writes[i].dstSet = *(m_pDescriptorSet);
                    writes[i].pBufferInfo = updateDescriptorSetInfos[i].bufferInfos.data();
                    writes[i].pImageInfo = updateDescriptorSetInfos[i].imageInfos.data();
                }
                auto pDevice = pApp->getDevice();
                pDevice->updateDescriptorSets(writes, nullptr);
                m_updateDescriptorSetInfos = updateDescriptorSetInfos;
            }

            m_descriptorSetChanged = VG_FALSE;
        }
    }

    void BindingSet::beginRecord() const
    {
#ifdef DEBUG
        if (m_dataChanged || m_dataContentChanged || m_textureChanged || m_bufferChanged || m_descriptorSetChanged)
            throw std::runtime_error("Pass should apply change before used to render.");
#endif //DEBUG
    }
        
    void BindingSet::endRecord() const
    {

    }

    void BindingSet::_updateDescriptorSetStateID()
    {
        ++m_descriptorSetStateID;
        if ( m_descriptorSetStateID == std::numeric_limits<DescriptorSetStateID>::max())
        {
            m_descriptorSetStateID = 1;
        }
    }
    
} //vg