#include "graphics/pass/pass.hpp"

namespace vg
{
    Pass::BuildInDataInfo::BuildInDataInfo()
        : componentCount(0u)
        , pComponent(nullptr)
    {

    }

    const std::array<uint32_t, static_cast<size_t>(Pass::BuildInDataType::COUNT)> Pass::buildInDataTypeSizes = {
        sizeof(BuildInDataTypeTypeInfo<BuildInDataType::MATRIX_OBJECT_TO_NDC>::Type),
        sizeof(BuildInDataTypeTypeInfo<BuildInDataType::MAIN_CLOLOR>::Type),
        sizeof(BuildInDataTypeTypeInfo<BuildInDataType::MATRIX_OBJECT_TO_WORLD>::Type),
        sizeof(BuildInDataTypeTypeInfo<BuildInDataType::MATRIX_OBJECT_TO_VIEW>::Type),
        sizeof(BuildInDataTypeTypeInfo<BuildInDataType::MATRIX_VIEW>::Type),
        sizeof(BuildInDataTypeTypeInfo<BuildInDataType::MATRIX_PROJECTION>::Type)
    };

    Pass::PushConstantUpdateInfo::PushConstantUpdateInfo(vk::ShaderStageFlags stageFlags
        , uint32_t offset
        , uint32_t size
        , const void *pData
        )
        : stageFlags(stageFlags)
        , offset(offset)
        , size(size)
        , pData(pData)
    {

    }

    Pass::VertexInputFilterInfo::VertexInputFilterInfo(Bool32 filterEnable
        , uint32_t locationCount
        , uint32_t * pBindings
        )
        : filterEnable(filterEnable)
        , locationCount(locationCount)
        , pLocations(pLocations)
    {

    }

    Bool32 Pass::_compareDataInfo(const DataSortInfo &item1, const DataSortInfo &item2)
    {
        uint32_t shaderStageValue1 = static_cast<uint32_t>(item1.shaderStageFlags);
        uint32_t shaderStageValue2 = static_cast<uint32_t>(item2.shaderStageFlags);
        int32_t result = static_cast<int32_t>(shaderStageValue1) - static_cast<int32_t>(shaderStageValue2);
        if (result > 0) {
            return VG_FALSE;
        } else if (result < 0) {
            return VG_TRUE;
        } else {
            return item1.layoutPriority < item2.layoutPriority;
        }
    }

    Bool32 Pass::_compareBufferTextureInfo(const BufferTextureSortInfo &item1, const BufferTextureSortInfo &item2)
    {
        return item1.bindingPriority < item2.bindingPriority;
    }

    Bool32 Pass::_compareExtUniformBufferInfo(const PassExtUniformBufferInfo &item1, const PassExtUniformBufferInfo &item2)
    {
        return item1.bindingPriority < item2.bindingPriority;
    }

    Bool32 Pass::_comparePushConstantInfo(const PushConstantSortInfo &item1, const PushConstantSortInfo &item2)
    {
        return item1.priority < item2.priority;
    }

    Bool32 Pass::_compareSpecializationSortInfo(const SpecializationSortInfo &item1, const SpecializationSortInfo &item2)
    {
        return item1.priority < item2.priority;
    }

    Pass::Pass() 
        : Base(BaseType::PASS)
        , m_data()
        , m_dataChanged(VG_FALSE)
        , m_dataContentChanged(VG_FALSE)
        , m_dataContentChanges()
        , m_textureChanged(VG_FALSE)
        , m_bufferChanged(VG_FALSE)
        , m_extUniformBufferChanged(VG_FALSE)
        , m_polygonMode()
        , m_cullMode()
        , m_frontFace()
        , m_viewport()
        , m_scissor()
        , m_depthStencilInfo()
        , m_colorBlendAttachmentStates()
        , m_colorBlendInfo()
        , m_lineWidth(1.0f)
        , m_instanceCount(1u)
        , m_subpass(0u)
        , m_defaultInputAssemblyState()
        , m_specializationChanged(VG_FALSE)
        , m_mapSpecilizationDatas()
        , m_mapSpecializationDataChanges()
        , m_pushConstant()
        , m_buildInDataInfo()
        , m_buildInDataInfoComponents()
        , m_buildInDataCache()
        , m_vertexInputFilterInfo()
        , m_vertexInputFilterLocations()

        //Applied
        , m_sortDataSet(_compareDataInfo)
        , m_dataBuffer(vk::BufferUsageFlagBits::eUniformBuffer
            , vk::MemoryPropertyFlagBits::eHostVisible)
        , m_sortBufferTexInfosSet(_compareBufferTextureInfo)
        , m_descriptorSetChanged(VG_FALSE)
        , m_layoutBindingCount()
        , m_descriptorSetLayoutBindings()
        , m_sortExtUniformBufferInfoSet(_compareExtUniformBufferInfo)
        , m_updateDescriptorSetInfos()
        , m_pDescriptorSetLayout(nullptr)
        , m_poolSizeInfos()
        , m_pDescriptorPool(nullptr)
        , m_pDescriptorSet(nullptr)
        , m_descriptorSetsChanged(VG_FALSE)
        , m_descriptorSetLayouts()
        , m_descriptorSets()
        , m_dynamicOffsetsChanged(VG_FALSE)
        , m_dynamicOffsets()
        , m_pushConstantChanged(VG_FALSE)
        , m_pushConstantRanges()
        , m_sortedPushConstantItems()
        , m_mapSpecializationAppliedData()
        , m_pPipelineLayout(nullptr)
        , m_pipelineLayoutChanged(VG_FALSE)
        , m_pipelineStateID()

        , m_pShader(nullptr)
        
    {
        _initDefaultBuildInDataInfo();
        _initBuildInData();
    }

    Pass::Pass(Shader *pShader)
        : Pass()
    {
        m_pShader = pShader;
    }


    Pass::~Pass()
    {
    }

    const Shader *Pass::getShader() const
    {
        return m_pShader;
    }
    
    Shader *Pass::getShader()
    {
        return m_pShader;
    }

    void Pass::setShader(Shader *pShader)
    {
        m_pShader = pShader;
        _updatePipelineStateID();
    }

    Bool32 Pass::hasBuffer(std::string name) const
    {
        return m_data.hasBuffer(name);
    }

    void Pass::addBuffer(std::string name, const PassBufferInfo &bufferInfo)
    {
        m_data.addBuffer(name, bufferInfo);
        m_bufferChanged = VG_TRUE;
    }

    void Pass::removeBuffer(std::string name)
    {
        m_data.removeBuffer(name);
        m_bufferChanged = VG_TRUE;
    }

    PassBufferInfo Pass::getBuffer(std::string name)
    {
        return m_data.getBufferInfo(name);
    }
        
    void Pass::setBuffer(std::string name, const PassBufferInfo &bufferInfo)
    {
        m_data.setBuffer(name, bufferInfo);
        m_bufferChanged = VG_TRUE;
    }

    Bool32 Pass::hasTexture(std::string name) const
    {
        return m_data.hasTexture(name);
    }

    void Pass::addTexture(std::string name, const PassTextureInfo &texInfo)
    {
        m_data.addTexture(name, texInfo);
        m_textureChanged = VG_TRUE;
    }

    void Pass::removeTexture(std::string name)
    {
        m_data.removeTexture(name);
        m_textureChanged = VG_TRUE;
    }

    PassTextureInfo Pass::getTexture(std::string name) const
    {
        return m_data.getTextureInfo(name);
    }
    
    void Pass::setTexture(std::string name, const PassTextureInfo &texInfo)
    {
        m_data.setTexture(name, texInfo);
        m_textureChanged = VG_TRUE;
    }

    Bool32 Pass::hasExtUniformBuffer(std::string name) const
    {
        return m_data.hasExtUniformBuffer(name);
    }
        
    void Pass::addExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &info)
    {
        m_data.addExtUniformBuffer(name, info);
        m_extUniformBufferChanged = VG_TRUE;
    }
        
    void Pass::removeExtUniformBuffer(std::string name)
    {
        m_data.removeExtUniformBuffer(name);
        m_extUniformBufferChanged = VG_TRUE;
    }

    PassExtUniformBufferInfo Pass::getExtUniformBuffer(std::string name) const
    {
        return m_data.getExtUniformBuffer(name);
    }

    void Pass::setExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &info)
    {
        m_data.setExtUniformBuffer(name, info);
        m_extUniformBufferChanged = VG_TRUE;
    }

    Bool32 Pass::hasData(std::string name) const
    {
        return m_data.hasData(name);
    }

    void Pass::removeData(std::string name)
    {
        m_data.removeData(name);
        m_dataChanged = VG_TRUE;
    }

    void Pass::addData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo)
    {
        m_data.addData(name, info, sizeInfo);
        m_dataChanged;
    }

    void Pass::addData(const std::string name, const PassDataInfo &info, void *src, uint32_t size)
    {
        m_data.addData(name, info, src, size);
        m_dataChanged = VG_TRUE;
    }
    
    void Pass::getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const
    {
        return m_data.getData(name, dst, size, offset);
    }

    void Pass::setData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo)
    {
        m_data.setData(name, info, sizeInfo);
        m_dataChanged = VG_TRUE;
    }
        
    void Pass::setData(const std::string name, void *src, uint32_t size, uint32_t offset)
    {
        m_data.setData(name, src, size, offset);
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[name] = VG_TRUE;
    }

    Color Pass::getMainColor() const
    {
        return m_buildInDataCache.mainColor;
    }

    void Pass::setMainColor(Color color)
    {
        // m_buildInDataCache.mainColor = color;
        _updateBuildInData(BuildInDataType::MAIN_CLOLOR, color);
    }

     void Pass::setBuildInDataInfo(BuildInDataInfo info)
     {
        m_buildInDataInfo = info;
        m_buildInDataInfoComponents.resize(info.componentCount);
        memcpy(m_buildInDataInfoComponents.data(), info.pComponent, 
            static_cast<size_t>(info.componentCount) * sizeof(BuildInDataInfo::Component));
        m_buildInDataInfo.pComponent = m_buildInDataInfoComponents.data();
        _initBuildInData();
     }

     const Pass::BuildInDataInfo &Pass::getBuildInDataInfo() const
     {
         return m_buildInDataInfo;
     }

    void Pass::setBuildInMatrixData(BuildInDataType type, Matrix4x4 matrix)
    {
        _updateBuildInData(type, matrix);
    }


    vk::PolygonMode Pass::getPolygonMode() const
    {
        return m_polygonMode;
    }

    void Pass::setPolygonMode(vk::PolygonMode polygonMode)
    {
        m_polygonMode = polygonMode;
        _updatePipelineStateID();
    }

    vk::CullModeFlags Pass::getCullMode() const
    {
        return m_cullMode;
    }

    void Pass::setCullMode(vk::CullModeFlags cullMode)
    {
        m_cullMode = cullMode;
        _updatePipelineStateID();
    }

    vk::FrontFace Pass::getFrontFace() const
    {
        return m_frontFace;
    }

    void Pass::setFrontFace(vk::FrontFace frontFace)
    {
        m_frontFace = frontFace;
        _updatePipelineStateID();
    }

    float Pass::getLineWidth() const
    {
        return m_lineWidth;
    }

    void Pass::setLineWidth(float lineWidth)
    {
        m_lineWidth = lineWidth;
    }

    const fd::Viewport &Pass::getViewport() const
    {
        return m_viewport;
    }

    void Pass::setViewport(const fd::Viewport &viewport)
    {
#ifdef DEBUG
        if (viewport.width < 0)
            throw std::invalid_argument("The width of viewport is smaller than 0!");
        else if (viewport.width > 1)
            throw std::invalid_argument("The width of viewport is bigger than 1!");
        if(viewport.height < 0)
            throw std::invalid_argument("The height of viewport is smaller than 0!");
        else if(viewport.height > 1)
            throw std::invalid_argument("The height of viewport is bigger than 1!");
        if(viewport.maxDepth < 0)
            throw std::invalid_argument("The maxDepth of viewport is smaller than 0!");
        else if(viewport.maxDepth > 1)
            throw std::invalid_argument("The maxDepth of viewport is bigger than 1!");

        if (viewport.x < 0)
            throw std::invalid_argument("the x of viewport is smaller than 0!");
        else if (viewport.x + viewport.width > 1)
            throw std::invalid_argument("The max x of viewport is bigger than 1!");
        if (viewport.y < 0)
            throw std::invalid_argument("the y of viewport is smaller than 0!");
        else if(viewport.y + viewport.height > 1)
            throw std::invalid_argument("The max y of viewport is bigger than 1!");
        if(viewport.minDepth < 0)
            throw std::invalid_argument("the minDepth of viewport is smaller than 0!");
        else if(viewport.minDepth > viewport.maxDepth)
            throw std::invalid_argument("The minDepth of viewport is bigger than the maxDepth of viewport!");
#endif // DEBUG
        if (m_viewport == viewport) return;
        m_viewport = viewport;
        _updatePipelineStateID();
    }

    const fd::Rect2D &Pass::getScissor() const
    {
        return m_scissor;
    }

    void Pass::setScissor(const fd::Rect2D &scissor)
    {
#ifdef DEBUG
        if (scissor.width < 0)
            throw std::invalid_argument("The width of scissor is smaller than 0!");
        else if (scissor.width > 1)
            throw std::invalid_argument("The width of scissor is bigger than 1!");
        if (scissor.height < 0)
            throw std::invalid_argument("The height of scissor is smaller than 0!");
        else if (scissor.height > 1)
            throw std::invalid_argument("The height of scissor is bigger than 1!");
        if (scissor.x < 0)
            throw std::invalid_argument("the x of scissor is smaller than 0!");
        else if (scissor.x > scissor.width)
            throw std::invalid_argument("The x of scissor is bigger than the width of scissor!");
        if (scissor.y < 0)
            throw std::invalid_argument("the y of scissor is smaller than 0!");
        else if (scissor.y > scissor.height)
            throw std::invalid_argument("The y of scissor is bigger than the height of scissor!");
#endif // DEBUG
        if (m_scissor == scissor) return;
        m_scissor = scissor;
        _updatePipelineStateID();
    }

    const vk::PipelineDepthStencilStateCreateInfo &Pass::getDepthStencilInfo() const
    {
        return m_depthStencilInfo;
    }

    void Pass::setDepthStencilInfo(const vk::PipelineDepthStencilStateCreateInfo &value)
    {
        m_depthStencilInfo = value;
        _updatePipelineStateID();
    }

    const vk::PipelineColorBlendStateCreateInfo &Pass::getColorBlendInfo() const
    {
        return m_colorBlendInfo;
    }

    void Pass::setColorBlendInfo(const vk::PipelineColorBlendStateCreateInfo &value)
    {
        //copy arguments.
        uint32_t count = value.attachmentCount;
        size_t size = count * sizeof(vk::PipelineColorBlendAttachmentState);
        m_colorBlendAttachmentStates.resize(count);
        memcpy(m_colorBlendAttachmentStates.data(), value.pAttachments, size);
        m_colorBlendInfo = value;
        m_colorBlendInfo.pAttachments = m_colorBlendAttachmentStates.data();
        m_colorBlendInfo.pNext = nullptr;
        _updatePipelineStateID();
    }

    const vk::PipelineInputAssemblyStateCreateInfo &Pass::getDefaultInputAssemblyState() const
    {
        return m_defaultInputAssemblyState;
    }

    void Pass::setDefaultInputAssemblyState(vk::PipelineInputAssemblyStateCreateInfo & value)
    {
        m_defaultInputAssemblyState = value;
    }

     Bool32 Pass::hasSpecializationInfo(vk::ShaderStageFlagBits shaderStage) const
     {
        const auto &iterator = m_mapSpecializationAppliedData.find(shaderStage);
        if (iterator == m_mapSpecializationAppliedData.cend())
        {
            return VG_FALSE;
        }
        else
        {
            return VG_TRUE;
        }
     }

    const vk::SpecializationInfo &Pass::getSpecializationInfo(vk::ShaderStageFlagBits shaderStage) const
    {
        const auto &iterator = m_mapSpecializationAppliedData.find(shaderStage);
        if (iterator == m_mapSpecializationAppliedData.cend())
        {
            throw std::runtime_error("Don't has the specialization info, its shader stage is : " + 
                std::to_string(static_cast<uint32_t>(shaderStage)));
        }
        else
        {
            return iterator->second.info;
        }
    }

    const PassSpecializationData &Pass::getSpecializationData(vk::ShaderStageFlagBits shaderStage) const
    {
        const auto &iterator = m_mapSpecilizationDatas.find(shaderStage);
        if (iterator == m_mapSpecilizationDatas.cend())
        {
            throw std::runtime_error("Don't has the specialization data, its shader stage is : " + 
                std::to_string(static_cast<uint32_t>(shaderStage)));
        }
        else
        {
            return iterator->second;
        }
    }

    const PassSpecializationData &Pass::getSpecializationData(vk::ShaderStageFlagBits shaderStage)
    {
        return m_mapSpecilizationDatas[shaderStage];
    }

    Bool32 Pass::hasSpecializationItem(vk::ShaderStageFlagBits shaderStage, std::string name) const
    {
        const auto &iterator = m_mapSpecilizationDatas.find(shaderStage);
        if (iterator == m_mapSpecilizationDatas.cend())
        {
            return VG_FALSE;
        }
        else
        {
            return iterator->second.hasSpecialization(name);
        }
    }
    
    void Pass::addSpecializationItem(vk::ShaderStageFlagBits shaderStage
        , std::string name
        , uint32_t priority
        , const void *pData
        , uint32_t size
        )
    {
        m_mapSpecilizationDatas[shaderStage].addSpecialization(name, priority, pData, size);
        m_specializationChanged = VG_TRUE;
        m_mapSpecializationDataChanges[shaderStage] = VG_TRUE;
    }

    void Pass::removeSpecializationItem(vk::ShaderStageFlagBits shaderStage, std::string name)
    {
        m_mapSpecilizationDatas[shaderStage].removeSpecialization(name);
        m_specializationChanged = VG_TRUE;
        m_mapSpecializationDataChanges[shaderStage] = VG_TRUE;
    }

    void Pass::setSpecializationItem(vk::ShaderStageFlagBits shaderStage
        , std::string name
        , uint32_t priority
        , const void *pData
        , uint32_t size
        )
    {
        m_mapSpecilizationDatas[shaderStage].setSpecialization(name, priority, pData, size);
        m_specializationChanged = VG_TRUE;
        m_mapSpecializationDataChanges[shaderStage] = VG_TRUE;
    }

    Bool32 Pass::hasPushConstant(std::string name) const
    {
        return m_pushConstant.hasPushConstant(name);
    }
        
    void Pass::addPushConstant(std::string name
        , uint32_t priority
        , vk::ShaderStageFlags stageFlags
        , uint32_t size
        )
    {
        m_pushConstant.addPushConstant(name, priority, stageFlags, size);
        m_pushConstantChanged = VG_TRUE;
    }

    void Pass::removePushConstant(std::string name)
    {
        m_pushConstant.removePushConstant(name);
        m_pushConstantChanged = VG_TRUE;
    }

    const PassPushConstantData::ConstantItem &Pass::getPushConstant(std::string name) const
    {
        return m_pushConstant.getPushConstant(name);
    }

    void Pass::setPushConstant(std::string name
        , uint32_t priority
        , vk::ShaderStageFlags stageFlags
        , uint32_t size
        )
    {
        m_pushConstant.setPushConstant(name, priority, stageFlags, size);
        m_pushConstantChanged = VG_TRUE;
    }

    void Pass::setPushConstantUpdate(std::string name
        , uint32_t offset
        , const void *pData
        , uint32_t size
        )
    {
        m_pushConstant.setPushConstantUpdate(name, offset, pData, size);
    }

    std::vector<Pass::PushConstantUpdateInfo> Pass::getPushconstantUpdates() const
    {
        std::vector<PushConstantUpdateInfo> updateInfos(m_sortedPushConstantItems.size());
        uint32_t index = 0u;
        for (const auto &item : m_sortedPushConstantItems)
        {
            const auto &pushConstantItem = m_pushConstant.getPushConstant(item.name);
            PushConstantUpdateInfo updateInfo = {
                pushConstantItem.getStageFlags(),
                pushConstantItem.getUpdate().getOffset(),
                pushConstantItem.getUpdate().getSize(),
                pushConstantItem.getUpdate().getData(),
            };
            updateInfos[index] = updateInfo;
            ++index;
        }
        return updateInfos;
    }

    uint32_t Pass::getInstanceCount() const
    {
        return m_instanceCount;
    }

    void Pass::setInstanceCount(uint32_t count)
    {
        m_instanceCount = count;
    }

    uint32_t Pass::getSubpass() const
    {
        return m_subpass;
    }

    void Pass::setSubpass(uint32_t subpass)
    {
        m_subpass = subpass;
    }

    const Pass::VertexInputFilterInfo &Pass::getVertexInputFilter() const
    {
        return m_vertexInputFilterInfo;
    }

    void Pass::setVertexInputFilterInfo(const VertexInputFilterInfo &value)
    {
        if (m_vertexInputFilterInfo.filterEnable == value.filterEnable && 
            m_vertexInputFilterInfo.locationCount == value.locationCount)
        {
            uint32_t count = value.locationCount;
            Bool32 isAllSame = VG_TRUE;
            for (uint32_t i = 0; i < count; ++i)
            {
                if (*(m_vertexInputFilterInfo.pLocations + i) != *(value.pLocations + i))
                {
                    isAllSame = VG_FALSE;
                    break;
                }
            }
            if (isAllSame == VG_FALSE)
            {
                return;
            }
        }
        uint32_t locationCount = value.locationCount;
        m_vertexInputFilterLocations.resize(locationCount);
        memcpy(m_vertexInputFilterLocations.data(), value.pLocations, locationCount * sizeof(uint32_t));
        m_vertexInputFilterInfo = value;
        m_vertexInputFilterInfo.pLocations = m_vertexInputFilterLocations.data();
        _updatePipelineStateID();
    }

    const BufferData &Pass::getBufferData() const
    {
        return m_dataBuffer;
    }

    const vk::DescriptorSetLayout *Pass::getDescriptorSetLayout() const
    {
        return m_pDescriptorSetLayout.get();
    }

    const vk::DescriptorPool *Pass::getDescriptorPool() const
    {
        return m_pDescriptorPool.get();
    }

    const vk::DescriptorSet *Pass::getDescriptorSet() const
    {
        return m_pDescriptorSet.get();
    }

    const vk::PipelineLayout *Pass::getPipelineLayout() const
    {
        return m_pPipelineLayout.get();
    }

    uint32_t Pass::getUsingDescriptorSetCount() const
    {
        return static_cast<uint32_t>(m_descriptorSets.size());
    }
        
    const vk::DescriptorSet *Pass::getUsingDescriptorSets() const
    {
        return m_descriptorSets.data();
    }

    uint32_t Pass::getUsingDescriptorDynamicOffsetCount() const
    {
        return static_cast<uint32_t>(m_dynamicOffsets.size());
    }

    const uint32_t *Pass::getUsingDescriptorDynamicOffsets() const
    {
        return m_dynamicOffsets.data();
    }

    Pass::PipelineStateID Pass::getPipelineStateID() const
    {
        return m_pipelineStateID;
    }

    void Pass::apply()
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

                //The Data change will make data content change.
                /*m_dataContentChanged = VG_TRUE;
                for (const auto &name : arrDataNames)
                {
                    m_dataContentChanges[name] = VG_TRUE;
                }*/
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

        if (m_bufferChanged || m_textureChanged) {
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
            m_layoutBindingCount = 0u;
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
                        const auto &textureData = *(reinterpret_cast<const PassTextureData *>(sortInfo.pData));
                        bindingInfo.descriptorType = tranImageDescriptorTypeToVK(textureData.descriptorType);
                        bindingInfo.stageFlags = textureData.stageFlags;
                        auto textureCount = static_cast<uint32_t>(textureData.textures.size());
                        bindingInfo.descriptorCount = textureCount;
                        updateDesSetInfo.imageInfos.resize(textureCount);
                        for (uint32_t i = 0; i < textureCount; ++i)
                        {
                            auto oneTexInfo = textureData.textures[i];
                            const auto pTexture = oneTexInfo.pTexture != nullptr ? oneTexInfo.pTexture : pDefaultTexture2D.get();
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
                        const auto &bufferData = *(reinterpret_cast<const PassBufferData *>(sortInfo.pData));
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
                    //Create descriptor set layout will make pipeline layout change.
                    m_pipelineLayoutChanged = VG_TRUE;
                    //Create descriptor set layout will make descriptor sets change.
                    m_descriptorSetsChanged = VG_TRUE;
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
                    m_descriptorSetsChanged = VG_TRUE;
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

        if (m_extUniformBufferChanged) {
            const auto &names = m_data.getExtUniformBufferNames();
            m_sortExtUniformBufferInfoSet.clear();
            for (const auto &name : names) {
                const auto &extUniformBufferInfo = m_data.getExtUniformBuffer(name);
                m_sortExtUniformBufferInfoSet.insert(extUniformBufferInfo);
            }
            
            //This change will make descriptor sets change.
            m_descriptorSetsChanged = VG_TRUE;

            m_extUniformBufferChanged = VG_FALSE;
        }

        //Update descriptor sets and their layouts.
        if (m_descriptorSetsChanged) {
            auto pLayout = m_pDescriptorSetLayout;
            uint32_t layoutCount = pLayout != nullptr ? 1 : 0;
            for (const auto &extUniformbuffer : m_sortExtUniformBufferInfoSet)
            {
                uint32_t subDataOffset = extUniformbuffer.subDataOffset;
                uint32_t subDataCount = extUniformbuffer.subDataCount;
                auto pSubDatas = extUniformbuffer.pData->getSubDatas();
                for (uint32_t i = 0; i < subDataCount; ++i)
                {
                    auto &subData = *(pSubDatas + (i + subDataOffset));
                    if (subData.getDescriptorSetLayout() != nullptr)
                    {
                        ++layoutCount;
                    }
                }
            }
    
            std::vector<vk::DescriptorSetLayout> setLayouts(layoutCount);
            std::vector<vk::DescriptorSet> descriptorSets(layoutCount);
            uint32_t layoutIndex = 0u;
            if (pLayout != nullptr) {
                setLayouts[layoutIndex] = *pLayout;
                descriptorSets[layoutIndex] = *m_pDescriptorSet;
                ++layoutIndex;
            }
    
            for (const auto &extUniformbuffer : m_sortExtUniformBufferInfoSet)
            {
                uint32_t subDataOffset = extUniformbuffer.subDataOffset;
                uint32_t subDataCount = extUniformbuffer.subDataCount;
                auto pSubDatas = extUniformbuffer.pData->getSubDatas();
                for (uint32_t i = 0; i < subDataCount; ++i)
                {
                    auto &subData = *(pSubDatas + (i + subDataOffset));
                    if (subData.getDescriptorSetLayout() != nullptr)
                    {
                        setLayouts[layoutIndex] = *(subData.getDescriptorSetLayout());
                        descriptorSets[layoutIndex] = *(subData.getDescriptorSet());
                        ++layoutIndex;
                    }
                }
            }

            if (m_descriptorSetLayouts != setLayouts)
            {
                //Descriptor layouts change will make pipeline layout change.
                m_pipelineLayoutChanged = VG_TRUE;
                m_descriptorSetLayouts = setLayouts;
            }
            m_descriptorSets = descriptorSets;

            //This change will make dynamic offsets change.
            m_dynamicOffsetsChanged = VG_TRUE;

            m_descriptorSetsChanged = VG_FALSE;
        }

        if (m_dynamicOffsetsChanged == VG_TRUE)
        {
            _applyUniformBufferDynamicOffsets();
            m_dynamicOffsetsChanged = VG_FALSE;
        }

        //Update push contant.
        if (m_pushConstantChanged)
        {
            //push constants
            const auto &arrNames = m_pushConstant.getPushConstantNames();
            m_sortedPushConstantItems.clear();
            for (const auto &name : arrNames)
            {
                const auto &item = m_pushConstant.getPushConstant(name);
                PushConstantSortInfo sortInfo = {name,
                    item.getPriority(),
                    };
                m_sortedPushConstantItems.insert(sortInfo);
            }

            std::vector<vk::PushConstantRange> pushConstantRanges(m_sortedPushConstantItems.size());
            uint32_t offset = 0u;
            uint32_t index = 0u;
            for (const auto &sortInfo : m_sortedPushConstantItems)
            {
                const auto &item = m_pushConstant.getPushConstant(sortInfo.name);
                vk::PushConstantRange range = {
                     item.getStageFlags(),
                     offset,
                     item.getRange().size,
                };
                pushConstantRanges[index] = range;
                offset += item.getRange().size;
                ++index;
            }

            if (m_pushConstantRanges != pushConstantRanges) {
                m_pushConstantRanges = pushConstantRanges;
                //Push constant change will make pipeline layout change.
                m_pipelineLayoutChanged = VG_TRUE;
            }

            m_pushConstantChanged = VG_FALSE;
        }

        if (m_specializationChanged == VG_TRUE)
        {
            for(auto &pair : m_mapSpecializationDataChanges) {
                if (pair.second == VG_TRUE) {
                    const auto &shaderStage = pair.first;

                    const auto &specializationData = getSpecializationData(shaderStage);
                    auto &appliedData = m_mapSpecializationAppliedData[shaderStage];
                    //sort
                    auto &sortSet = appliedData.sortSet;
                    sortSet.clear();
                    const auto &arrNames = specializationData.getSpecializationNames();
                    for (const auto &name : arrNames)
                    {
                        const auto &item = specializationData.getSpecialization(name);
                        SpecializationSortInfo sortInfo = {
                            name,
                            item.getPriority(),
                            };
                        sortSet.insert(sortInfo);
                    }

                    //create vk specialization info.
                    auto &specializationInfo = appliedData.info;
                    auto &specializationMapEntries = appliedData.mapEntries;
                    auto &data = appliedData.data;

                    ////get total size.
                    uint32_t totalSize = 0u;
                    for (const auto &item : sortSet)
                    {
                        const auto &specailzationItem = specializationData.getSpecialization(item.name);
                        totalSize += specailzationItem.getSize();
                    }

                    specializationMapEntries.resize(sortSet.size());
                    data.resize(totalSize);

                    uint32_t index = 0u;
                    uint32_t offset = 0u;
                    for (const auto &item : sortSet)
                    {
                        const auto &specailzationItem = specializationData.getSpecialization(item.name);
                        specializationMapEntries[index].constantID = index;

                        memcpy(data.data() + offset, specailzationItem.getData(), specailzationItem.getSize());
                        
                        specializationMapEntries[index].offset = offset;
                        specializationMapEntries[index].size = specailzationItem.getSize();


                        offset += specailzationItem.getSize();
                        ++index;
                    }

                    specializationInfo.mapEntryCount = static_cast<uint32_t>(specializationMapEntries.size());
                    specializationInfo.pMapEntries = specializationMapEntries.data();
                    specializationInfo.dataSize = totalSize;
                    specializationInfo.pData = data.data();

                    pair.second = VG_FALSE;
                }
            }

            _updatePipelineStateID();
            m_specializationChanged = VG_FALSE;
        }

        //Create pipeline layout.
        if (m_pipelineLayoutChanged) {
            vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
                vk::PipelineLayoutCreateFlags(),                         //flags
                static_cast<uint32_t>(m_descriptorSetLayouts.size()),    //setLayoutCount
                m_descriptorSetLayouts.data(),                           //pSetLayouts
                static_cast<uint32_t>(m_pushConstantRanges.size()),       //pushConstantRangeCount
                m_pushConstantRanges.data()                              //pPushConstantRanges
            };

            auto pDevice = pApp->getDevice();
            m_pPipelineLayout = fd::createPipelineLayout(pDevice, pipelineLayoutCreateInfo);
            _updatePipelineStateID();

            m_pipelineLayoutChanged = VG_FALSE;
        }
    }

    void Pass::beginRecord() const
    {
#ifdef DEBUG
        if (m_dataChanged || m_dataContentChanged || m_textureChanged || m_bufferChanged || m_extUniformBufferChanged ||
            m_descriptorSetChanged || m_descriptorSetsChanged || m_dynamicOffsetsChanged || 
            m_specializationChanged || m_pipelineLayoutChanged)
            throw std::runtime_error("Pass should apply change before used to render.");
#endif //DEBUG
    }
        
    void Pass::endRecord() const
    {

    }

    void Pass::_initDefaultBuildInDataInfo()
    {
        m_buildInDataInfoComponents.resize(3);
        m_buildInDataInfoComponents[0].type = BuildInDataType::MATRIX_OBJECT_TO_NDC;
        m_buildInDataInfoComponents[1].type = BuildInDataType::MAIN_CLOLOR;
        m_buildInDataInfoComponents[2].type = BuildInDataType::MATRIX_OBJECT_TO_WORLD;

        m_buildInDataInfo.componentCount = static_cast<uint32_t>(m_buildInDataInfoComponents.size());
        m_buildInDataInfo.pComponent = m_buildInDataInfoComponents.data();
    }

    void Pass::_initBuildInData()
    {
        uint32_t size = 0u;
        auto componentCount = m_buildInDataInfo.componentCount;
        for (uint32_t componentIndex = 0; componentIndex < componentCount; ++componentIndex)
        {
            BuildInDataType type = (*(m_buildInDataInfo.pComponent + componentIndex)).type;
            uint32_t count = static_cast<uint32_t>(BuildInDataType::COUNT);
            for (uint32_t i = 0; i < count; ++i)
            {
                if (i == static_cast<uint32_t>(type))
                {
                    size += buildInDataTypeSizes[i];
                    break;
                }
            }
        }

        if (size > 0)
        {
            PassDataInfo info = {
                VG_PASS_BUILDIN_DATA_LAYOUT_PRIORITY,
                vk::ShaderStageFlagBits::eVertex,
            };
            PassDataSizeInfo sizeInfo = {
                size,
            };
            if (hasData(VG_PASS_BUILDIN_DATA_NAME) == VG_FALSE)
            {
                addData(VG_PASS_BUILDIN_DATA_NAME, info, sizeInfo);
            }
            else
            {
                setData(VG_PASS_BUILDIN_DATA_NAME, info, sizeInfo);
            }

            uint32_t offset1 = 0u;
            for (uint32_t componentIndex = 0; componentIndex < componentCount; ++componentIndex)
            {
                BuildInDataType type = (*(m_buildInDataInfo.pComponent + componentIndex)).type;
                uint32_t count = static_cast<uint32_t>(BuildInDataType::COUNT);
                uint32_t offset2= 0u;
                for (uint32_t i = 0; i < count; ++i)
                {
                    if (i == static_cast<uint32_t>(type))
                    {
                        setData(VG_PASS_BUILDIN_DATA_NAME
                            , ((char *)(&m_buildInDataCache) + offset2)
                            , buildInDataTypeSizes[static_cast<uint32_t>(type)]
                            , offset1);
                        break;
                    }
                    else
                    {
                        offset2 += buildInDataTypeSizes[i];
                    }
                }
                offset1 += buildInDataTypeSizes[static_cast<uint32_t>(type)];
            }
        }
        else if (m_data.hasData(VG_PASS_BUILDIN_DATA_NAME))
        {
            m_data.removeData(VG_PASS_BUILDIN_DATA_NAME);
        }
        
        
        m_dataChanged = VG_TRUE;
    }

    void Pass::_updatePipelineStateID()
    {
        ++m_pipelineStateID;
        if ( m_pipelineStateID == std::numeric_limits<PipelineStateID>::max())
        {
            m_pipelineStateID = 1;
        }
    }

    void Pass::_applyUniformBufferDynamicOffsets()
    {
        uint32_t dynamicCount = 0u;
        for (const auto &extUniformbuffer : m_sortExtUniformBufferInfoSet)
        {
            uint32_t subDataOffset = extUniformbuffer.subDataOffset;
            uint32_t subDataCount = extUniformbuffer.subDataCount;
            auto pSubDatas = extUniformbuffer.pData->getSubDatas();
            for (uint32_t i = 0; i < subDataCount; ++i)
            {
                auto &subData = *(pSubDatas + (i + subDataOffset));
                if (subData.getDescriptorSetLayout() != nullptr)
                {
                    for (uint32_t bindingIndex = 0u; bindingIndex < subData.getLayoutBindingCount(); ++bindingIndex)
                    {
                        const auto &binding = *(subData.getLayoutBindings() + bindingIndex);
                        if (binding.descriptorType == vk::DescriptorType::eUniformBufferDynamic ||
                            binding.descriptorType == vk::DescriptorType::eStorageBufferDynamic)
                        {
                            dynamicCount += binding.descriptorCount;
                        }
                    }
                }
            }
        }

        if (static_cast<uint32_t>(m_dynamicOffsets.size()) != dynamicCount)
              m_dynamicOffsets.resize(dynamicCount);
        uint32_t dynamicIndex = 0u;

        for (const auto &extUniformbuffer : m_sortExtUniformBufferInfoSet)
        {
            uint32_t subDataOffset = extUniformbuffer.subDataOffset;
            uint32_t subDataCount = extUniformbuffer.subDataCount;
            auto pSubDatas = extUniformbuffer.pData->getSubDatas();
            for (uint32_t i = 0; i < subDataCount; ++i)
            {
                auto &subData = *(pSubDatas + (i + subDataOffset));
                if (subData.getDescriptorSetLayout() != nullptr)
                {
                    uint32_t descriptorInfoIndex = 0u;
                    for (uint32_t bindingIndex = 0u; bindingIndex < subData.getLayoutBindingCount(); ++bindingIndex)
                    {
                        const auto &binding = *(subData.getLayoutBindings() + bindingIndex);
                        if (binding.descriptorType == vk::DescriptorType::eUniformBufferDynamic ||
                            binding.descriptorType == vk::DescriptorType::eStorageBufferDynamic)
                        {
                            for (uint32_t descriptorIndex = 0u; descriptorIndex < binding.descriptorCount; ++descriptorIndex)
                            {
                                auto &descriptorInfo = *(subData.getDescriptorInfos() + (descriptorInfoIndex + descriptorIndex));
                                if(m_dynamicOffsets[dynamicIndex] != descriptorInfo.dynamicOffset)
                                    m_dynamicOffsets[dynamicIndex] = descriptorInfo.dynamicOffset;
                                ++dynamicIndex;
                            }
                        }
                        descriptorInfoIndex += binding.descriptorCount;
                    }
    
                }
            }
        }
    }

    Pass::_BuildInDataCache::_BuildInDataCache()
        : matrixObjectToNDC(1.0f)
        , mainColor(1.0f, 1.0f, 1.0f, 1.0f)
        , matrixObjectToWorld(1.0f)
        , matrixObjectToView(1.0f)
        , matrixView(1.0f)
        , matrixProjection(1.0f)
    {

    }

    Pass::_BuildInDataCache::_BuildInDataCache(Matrix4x4 matrixObjectToNDC
        , Color mainColor
        , Matrix4x4 matrixObjectToWorld
        , Matrix4x4 matrixObjectToView
        , Matrix4x4 matrixView
        , Matrix4x4 matrixProjection)
        : matrixObjectToNDC(matrixObjectToNDC)
        , mainColor(mainColor)
        , matrixObjectToWorld(matrixObjectToWorld)
        , matrixObjectToView(matrixObjectToView)
        , matrixView(matrixView)
        , matrixProjection(matrixProjection)
    {

    }

    Pass::_BuildInDataCache::_BuildInDataCache(const _BuildInDataCache &target)
        : matrixObjectToNDC(target.matrixObjectToNDC)
        , mainColor(target.mainColor)
        , matrixObjectToWorld(target.matrixObjectToWorld)
        , matrixObjectToView(target.matrixObjectToView)
        , matrixView(target.matrixView)
        , matrixProjection(target.matrixProjection)
    {

    }

    Pass::_BuildInDataCache::_BuildInDataCache(const _BuildInDataCache &&target)
        : matrixObjectToNDC(target.matrixObjectToNDC)
        , mainColor(target.mainColor)
        , matrixObjectToWorld(target.matrixObjectToWorld)
        , matrixObjectToView(target.matrixObjectToView)
        , matrixView(target.matrixView)
        , matrixProjection(target.matrixProjection)
    {
    }

    Pass::DataSortInfo::DataSortInfo(std::string name
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

    Pass::BufferTextureSortInfo::BufferTextureSortInfo(std::string name
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

    Pass::UpdateDescriptorSetInfo::UpdateDescriptorSetInfo(std::vector<vk::DescriptorBufferInfo> bufferInfos
        , std::vector<vk::DescriptorImageInfo> imageInfos
        )
        :  bufferInfos(bufferInfos)
        , imageInfos(imageInfos)
    {

    }

    Pass::PushConstantSortInfo::PushConstantSortInfo(std::string name
        , uint32_t priority
        )
        : name(name)
        , priority(priority)
    {

    }

    Pass::SpecializationSortInfo::SpecializationSortInfo(std::string name
        , uint32_t priority
        )
        : name(name)
        , priority(priority)
    {

    }

    Pass::SpecializationAppliedData::SpecializationAppliedData()
        : sortSet(_compareSpecializationSortInfo)
        , info()
        , mapEntries()
        , data()
    {

    }
}