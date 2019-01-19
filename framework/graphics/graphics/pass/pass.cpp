#include "graphics/pass/pass.hpp"

namespace vg
{
    Pass::BuildInDataInfo::BuildInDataInfo(uint32_t componentCount, Component *pComponent)
        : componentCount(componentCount)
        , pComponent(pComponent)
    {

    }

    Pass::BuildInDataInfo::BuildInDataInfo(const BuildInDataInfo &target)
        : componentCount(target.componentCount)
        , pComponent(target.pComponent)
    {

    }
    
    Pass::BuildInDataInfo &Pass::BuildInDataInfo::operator=(const BuildInDataInfo &target)
    {
        componentCount = target.componentCount;
        pComponent = target.pComponent;
        return *this;
    }

    Bool32 Pass::BuildInDataInfo::operator==(const BuildInDataInfo &target) const
    {
        if (componentCount != target.componentCount) return VG_FALSE;
        auto count = componentCount;
        for (uint32_t i = 0; i < count; ++i) {
            if ((pComponent + i)->type != (target.pComponent + i)->type) {
                return VG_FALSE;
            }
        }
        return VG_TRUE;
    }

    Bool32 Pass::BuildInDataInfo::operator!=(const BuildInDataInfo &target) const
    {
        return ! ((*this) == target);
    }

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

    Pass::PushConstantUpdateInfo::PushConstantUpdateInfo(const PushConstantUpdateInfo &target)
        : stageFlags(target.stageFlags)
        , offset(target.offset)
        , size(target.size)
        , pData(target.pData)
    {
        
    }
            
    Pass::PushConstantUpdateInfo &Pass::PushConstantUpdateInfo::operator=(const PushConstantUpdateInfo &target)
    {
        stageFlags = target.stageFlags;
        offset = target.offset;
        size = target.size;
        pData = target.pData;
        return *this;
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

    Pass::VertexInputFilterInfo::VertexInputFilterInfo(const VertexInputFilterInfo &target)
        : filterEnable(target.filterEnable)
        , locationCount(target.locationCount)
        , pLocations(target.pLocations)
    {

    }
    
    Pass::VertexInputFilterInfo &Pass::VertexInputFilterInfo::operator=(const VertexInputFilterInfo &target)
    {
        filterEnable = target.filterEnable;
        locationCount = target.locationCount;
        pLocations = target.pLocations;
        return *this;
    }

    Pass::DepthBiasInfo::DepthBiasInfo(Bool32 enable
        , Bool32 dynamic
        , float constantFactor
        , float clamp
        , float slopeFactor
        )
        : enable(enable)
        , dynamic(dynamic)
        , constantFactor(constantFactor)
        , clamp(clamp)
        , slopeFactor(slopeFactor)
    {

    }
            
    Pass::DepthBiasInfo::DepthBiasInfo(const DepthBiasInfo &target)
        : enable(target.enable)
        , dynamic(target.dynamic)
        , constantFactor(target.constantFactor)
        , clamp(target.clamp)
        , slopeFactor(target.slopeFactor)
    {

    }
            
    Pass::DepthBiasInfo &Pass::DepthBiasInfo::operator=(const DepthBiasInfo &target)
    {
        enable = target.enable;
        dynamic = target.dynamic;
        constantFactor = target.constantFactor;
        clamp = target.clamp;
        slopeFactor = target.slopeFactor;
        return *this;
    }

    Bool32 Pass::DepthBiasInfo::operator==(const DepthBiasInfo &target) const
    {
        if (enable == target.enable && 
            dynamic == target.dynamic &&
            constantFactor == target.constantFactor &&
            clamp == target.clamp &&
            slopeFactor == target.slopeFactor
            ) return VG_TRUE;
        return VG_FALSE;
    }

    Bool32 Pass::DepthBiasInfo::operator!=(const DepthBiasInfo &target) const
    {
        return ! ((*this) == target);
    }

    Pass::DepthBiasUpdateInfo::DepthBiasUpdateInfo(float constantFactor
        , float clamp
        , float slopeFactor
        )
        : constantFactor(constantFactor)
        , clamp(clamp)
        , slopeFactor(slopeFactor)
    {

    }
    
    Pass::DepthBiasUpdateInfo::DepthBiasUpdateInfo(const DepthBiasUpdateInfo &target)
        : constantFactor(target.constantFactor)
        , clamp(target.clamp)
        , slopeFactor(target.slopeFactor)
    {

    }
            
    Pass::DepthBiasUpdateInfo &Pass::DepthBiasUpdateInfo::operator=(const DepthBiasUpdateInfo &target)
    {
        constantFactor = target.constantFactor;
        clamp = target.clamp;
        slopeFactor = target.slopeFactor;
        return *this;
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
        , m_bindingSet()
        , m_bindingSetDescriptorSetStateID()
        , m_extUniformBuffers()
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
        , m_vertexInputFilterInfo()
        , m_vertexInputFilterLocations()
        , m_depthBiasInfo()
        , m_depthBiasUpdateInfo()

        //Applied
        , m_sortExtUniformBufferInfoSet(_compareExtUniformBufferInfo)
        , m_descriptorSetsChanged(VG_FALSE)
        , m_descriptorSetLayouts()
        , m_descriptorSets()
        , m_dynamicOffsetsChanged(VG_FALSE)
        , m_dynamicOffsets()
        , m_pushConstantChanged(VG_FALSE)
        , m_pushConstantRanges()
        , m_sortedPushConstantItems()
        , m_mapSpecializationAppliedData()
        , m_pipelineLayoutStateID()
        , m_pipelineStateID()

        , m_pShader(nullptr)
        
    {
        _initDefaultBuildInDataInfo();
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
        return m_bindingSet.hasBuffer(name);
    }

    void Pass::addBuffer(std::string name, const PassBufferInfo &bufferInfo)
    {
        m_bindingSet.addBuffer(name, bufferInfo);
    }

    void Pass::removeBuffer(std::string name)
    {
        m_bindingSet.removeBuffer(name);
    }

    PassBufferInfo Pass::getBuffer(std::string name)
    {
        return m_bindingSet.getBuffer(name);
    }
        
    void Pass::setBuffer(std::string name, const PassBufferInfo &bufferInfo)
    {
        m_bindingSet.setBuffer(name, bufferInfo);
    }

    Bool32 Pass::hasTexture(std::string name) const
    {
        return m_bindingSet.hasTexture(name);
    }

    void Pass::addTexture(std::string name, const PassTextureInfo &texInfo)
    {
        m_bindingSet.addTexture(name, texInfo);
    }

    void Pass::removeTexture(std::string name)
    {
        m_bindingSet.removeTexture(name);
    }

    PassTextureInfo Pass::getTexture(std::string name) const
    {
        return m_bindingSet.getTexture(name);
    }
    
    void Pass::setTexture(std::string name, const PassTextureInfo &texInfo)
    {
        m_bindingSet.setTexture(name, texInfo);
    }

    Bool32 Pass::hasExtUniformBuffer(std::string name) const
    {
        return m_extUniformBuffers.hasExtUniformBuffer(name);
    }
        
    void Pass::addExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &info)
    {
        m_extUniformBuffers.addExtUniformBuffer(name, info);
        m_extUniformBufferChanged = VG_TRUE;
    }
        
    void Pass::removeExtUniformBuffer(std::string name)
    {
        m_extUniformBuffers.removeExtUniformBuffer(name);
        m_extUniformBufferChanged = VG_TRUE;
    }

    PassExtUniformBufferInfo Pass::getExtUniformBuffer(std::string name) const
    {
        return m_extUniformBuffers.getExtUniformBuffer(name);
    }

    void Pass::setExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &info)
    {
        m_extUniformBuffers.setExtUniformBuffer(name, info);
        m_extUniformBufferChanged = VG_TRUE;
    }

    Bool32 Pass::hasData(std::string name) const
    {
        return m_bindingSet.hasData(name);
    }

    void Pass::removeData(std::string name)
    {
        m_bindingSet.removeData(name);
    }

    void Pass::addData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo)
    {
        m_bindingSet.addData(name, info, sizeInfo);
    }

    void Pass::addData(const std::string name, const PassDataInfo &info, const void *src, uint32_t size)
    {
        m_bindingSet.addData(name, info, src, size);
    }
    
    void Pass::getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const
    {
        return m_bindingSet.getData(name, dst, size, offset);
    }

    void Pass::setData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo)
    {
        m_bindingSet.setData(name, info, sizeInfo);
    }
        
    void Pass::setData(const std::string name, const void *src, uint32_t size, uint32_t offset)
    {
        m_bindingSet.setData(name, src, size, offset);
    }

    Color Pass::getMainColor() const
    {
        return m_mainColor;
    }

    void Pass::setMainColor(Color color)
    {
        m_mainColor = color;
    }

     void Pass::setBuildInDataInfo(BuildInDataInfo info)
     {
        m_buildInDataInfo = info;
        m_buildInDataInfoComponents.resize(info.componentCount);
        memcpy(m_buildInDataInfoComponents.data(), info.pComponent, 
            static_cast<size_t>(info.componentCount) * sizeof(BuildInDataInfo::Component));
        m_buildInDataInfo.pComponent = m_buildInDataInfoComponents.data();
        // _initBuildInData();
     }

     const Pass::BuildInDataInfo &Pass::getBuildInDataInfo() const
     {
         return m_buildInDataInfo;
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

    const std::vector<vk::PushConstantRange> &Pass::getPushConstantRanges() const
    {
        return m_pushConstantRanges;
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

    const Pass::DepthBiasInfo &Pass::getDepthBiasInfo() const
    {
        return m_depthBiasInfo;
    }

    void Pass::setDepthBiasInfo(const DepthBiasInfo &value)
    {
        if (m_depthBiasInfo != value) {
            m_depthBiasInfo = value;
            _updatePipelineStateID();
        }
    }

    const Pass::DepthBiasUpdateInfo &Pass::getDepthBiasUpdateInfo() const
    {
        return m_depthBiasUpdateInfo;
    }

    void Pass::setDepthBiasUpdateInfo(const DepthBiasUpdateInfo &value)
    {
        m_depthBiasUpdateInfo = value;
    }

    const BindingSet &Pass::getBindingSet() const
    {
        return m_bindingSet;
    }

    uint32_t Pass::getDescriptorSetLayoutCount() const
    {
        return static_cast<uint32_t>(m_descriptorSetLayouts.size());
    }

    const vk::DescriptorSetLayout *Pass::getDescriptorSetLayouts() const
    {
        return m_descriptorSetLayouts.data();
    }

    uint32_t Pass::getDescriptorSetCount() const
    {
        return static_cast<uint32_t>(m_descriptorSets.size());
    }
        
    const vk::DescriptorSet *Pass::getDescriptorSets() const
    {
        return m_descriptorSets.data();
    }

    uint32_t Pass::getDescriptorDynamicOffsetCount() const
    {
        return static_cast<uint32_t>(m_dynamicOffsets.size());
    }

    const uint32_t *Pass::getDescriptorDynamicOffsets() const
    {
        return m_dynamicOffsets.data();
    }

    Pass::PipelineLayoutStateID Pass::getPipelineLayoutStateID() const
    {
        return m_pipelineLayoutStateID;
    }

    Pass::PipelineStateID Pass::getPipelineStateID() const
    {
        return m_pipelineStateID;
    }

    void Pass::apply()
    {
        m_bindingSet.apply();
        if (m_bindingSetDescriptorSetStateID != m_bindingSet.getDescriptorSetStateID()) {

            m_descriptorSetsChanged = VG_TRUE;

            m_bindingSetDescriptorSetStateID = m_bindingSet.getDescriptorSetStateID();
        }

        if (m_extUniformBufferChanged) {
            const auto &names = m_extUniformBuffers.getExtUniformBufferNames();
            m_sortExtUniformBufferInfoSet.clear();
            for (const auto &name : names) {
                const auto &extUniformBufferInfo = m_extUniformBuffers.getExtUniformBuffer(name);
                m_sortExtUniformBufferInfoSet.insert(extUniformBufferInfo);
            }
            
            //This change will make descriptor sets change.
            m_descriptorSetsChanged = VG_TRUE;

            m_extUniformBufferChanged = VG_FALSE;
        }

        //Update descriptor sets and their layouts.
        if (m_descriptorSetsChanged) {
            auto pLayout = m_bindingSet.getDescriptorSetLayout();
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
                descriptorSets[layoutIndex] = *(m_bindingSet.getDescriptorSet());
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
                _updatePipelineLayoutStateID();
                _updatePipelineStateID();
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
                _updatePipelineLayoutStateID();
                _updatePipelineStateID();
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
    }

    void Pass::beginRecord() const
    {
		m_bindingSet.beginRecord();
#ifdef DEBUG
        if (m_extUniformBufferChanged || m_descriptorSetsChanged || m_dynamicOffsetsChanged || 
            m_specializationChanged)
            throw std::runtime_error("Pass should apply change before used to render.");
#endif //DEBUG
    }
        
    void Pass::endRecord() const
    {
		m_bindingSet.endRecord();
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

    void Pass::_updatePipelineLayoutStateID()
    {
        ++m_pipelineLayoutStateID;
        if ( m_pipelineStateID == std::numeric_limits<PipelineLayoutStateID>::max())
        {
            m_pipelineStateID = 1;
        }
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