#include "graphics/pass/pass.hpp"

namespace vg
{

	Pass::ExternalUniformBufferInfo::ExternalUniformBufferInfo(UniformBufferData *pData
		, uint32_t subDataOffset
		, uint32_t subDataCount)
		: pData(pData)
		, subDataOffset(subDataOffset)
		, subDataCount(subDataCount)
	{

	}

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

	Pass::SpecializationData::SpecializationData()
	    : m_mapEntries()
		, m_info()
		, m_size()
		, m_pData(nullptr)
	{

	}

	Pass::SpecializationData::~SpecializationData()
	{
		if (m_pData != nullptr)
		{
			free(m_pData);
		}
	}

	Pass::SpecializationData::SpecializationData(const SpecializationData & target)
	{
		init(target.m_info);
	}

	Pass::SpecializationData::SpecializationData(const SpecializationData && target)
	    : m_mapEntries(std::move(target.m_mapEntries))
		, m_info(std::move(target.m_info))
		, m_size(std::move(target.m_size))
		, m_pData(std::move(target.m_pData))
	{

	}

	Pass::SpecializationData& Pass::SpecializationData::operator=(const SpecializationData & target)
	{
		init(target.m_info);
		return *this;
	}

	void Pass::SpecializationData::init(const vk::SpecializationInfo &info)
	{
		uint32_t count = info.mapEntryCount;
		size_t sizeOfSrcMap = count * sizeof(vk::SpecializationMapEntry);
		m_info.mapEntryCount = count;
		m_mapEntries.resize(count);
		memcpy(m_mapEntries.data(), info.pMapEntries, sizeOfSrcMap);
        m_info.pMapEntries = m_mapEntries.data();
		if (m_pData != nullptr && (m_size < info.dataSize))
		{
			free(m_pData);
			m_pData = nullptr;
			m_size = 0;
		}
	
		if (m_pData == nullptr)
		{
			m_pData = malloc(info.dataSize);
			m_size = info.dataSize;
		}
		memcpy(m_pData, info.pData, info.dataSize);
		m_info.dataSize = info.dataSize;
		m_info.pData = m_pData;
	}

	const vk::SpecializationInfo Pass::SpecializationData::getInfo() const
	{
		return m_info;
	}

	const void * Pass::SpecializationData::getData() const
	{
		return m_pData;
	}

	uint32_t Pass::SpecializationData::getSize() const
	{
		return m_size;
	}

	Pass::PushConstantUpdate::PushConstantUpdate()
	    : m_stageFlags()
		, m_offset()
		, m_size()
		, m_pData(nullptr)
	{

	}
	
	Pass::PushConstantUpdate::~PushConstantUpdate()
	{
		if (m_pData != nullptr)
		{
			free(m_pData);
		}
	}

	void Pass::PushConstantUpdate::init(const void *pData
		, uint32_t size
		, vk::ShaderStageFlags stageFlags
		, uint32_t offset)
	{
		if (m_pData != nullptr && (m_size != size))
		{
			free(m_pData);
			m_pData = nullptr;
			m_size = 0;
		}
	
		if (m_pData == nullptr)
		{
			m_pData = malloc(size);
			m_size = size;
		}
		memcpy(m_pData, pData, size);

		m_stageFlags = stageFlags;
		m_offset = offset;
	}

	vk::ShaderStageFlags Pass::PushConstantUpdate::getStageFlags() const
	{
		return m_stageFlags;
	}

	uint32_t Pass::PushConstantUpdate::getOffset() const
	{
		return m_offset;
	}

	const void *Pass::PushConstantUpdate::getData() const
	{
		return m_pData;
	}

	uint32_t Pass::PushConstantUpdate::getSize() const
	{
		return m_size;
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


	Pass::Pass() 
		: Base(BaseType::PASS)
		, m_data()
		, m_dataChanged(VG_FALSE)
		, m_textureChanged(VG_FALSE)
		, m_bufferChanged(VG_FALSE)
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
		, m_mapSpecilizationDatas()
		, m_mapPushConstantRanges()
		, m_arrPushConstantRangeNames()
		, m_mapPPushConstantUpdates()		
		, m_arrPushConstantUpdateNames()
		, m_buildInDataInfo()
		, m_buildInDataInfoComponents()
		, m_buildInDataCache()
		, m_extUniformBufferCount()
		, m_extUniformBuffers()
		, m_vertexInputFilterInfo()
		, m_vertexInputFilterLocations()

		, m_dataBuffer()
	    , m_layoutBindingCount()
		, m_layoutBindings()
		, m_pDescriptorSetLayout(nullptr)
		, m_poolSizeInfos()
		, m_pDescriptorPool(nullptr)
		, m_pDescriptorSet(nullptr)
		, m_descriptorSetsChanged(VG_FALSE)
		, m_descriptorSetLayouts()
		, m_descriptorSets()
		, m_dynamicOffsets()
		, m_pPipelineLayout(nullptr)
		, m_pipelineLayoutChanged(VG_FALSE)
		, m_pipelineStateID()
		
	{
		_initDefaultBuildInDataInfo();
		_initBuildInData();
	}

	Pass::Pass(Shader *pShader)
		: Pass()
	{
		m_pShader = pShader;
		_initDefaultBuildInDataInfo();
		_initBuildInData();
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

	void Pass::addBuffer(std::string name, PassBufferInfo bufferInfo)
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
		return m_data.getBuffer(name);
	}
		
	void Pass::setBuffer(std::string name, PassBufferInfo bufferInfo)
	{
		m_data.setBuffer(name, bufferInfo);
		m_bufferChanged = VG_TRUE;
	}

	Bool32 Pass::hasTexture(std::string name) const
	{
		return m_data.hasTexture(name);
	}

	void Pass::addTexture(std::string name, PassTextureInfo texInfo)
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
		return m_data.getTexture(name);
	}
	
	void Pass::setTexture(std::string name, PassTextureInfo texInfo)
	{
		m_data.setTexture(name, texInfo);
		m_textureChanged = VG_TRUE;
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

	void Pass::addData(const std::string name, const PassDataInfo &info, void *src, uint32_t size)
	{
		m_data.addData(name, info, src, size);
		m_dataChanged = VG_TRUE;
	}
	
	void Pass::getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const
	{
		return m_data.getData(name, dst, size, offset);
	}
		
	void Pass::setData(const std::string name, void *src, uint32_t size, uint32_t offset)
	{
		m_data.setData(name, src, size, offset);
		m_dataChanged = VG_TRUE;
	}

	Color Pass::getMainColor() const
	{
		return m_buildInDataCache.mainColor;
	}

	void Pass::setMainColor(Color color)
	{
		m_buildInDataCache.mainColor = color;
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

	const Bool32 Pass::IsHasSpecializationData(vk::ShaderStageFlagBits shaderStage) const
	{
		const auto& map = m_mapSpecilizationDatas;
		return map.count(shaderStage) != 0;
	}

	const Pass::SpecializationData *Pass::getSpecializationData(vk::ShaderStageFlagBits shaderStage) const
	{
		const auto& map = m_mapSpecilizationDatas;
		const auto& iterator = map.find(shaderStage);
		if (iterator == map.cend())
		{
			throw std::runtime_error("SpecializationData don't exit.");
		}
		else
		{
			return iterator->second.get();
		}
	}

	const std::unordered_map<vk::ShaderStageFlagBits, std::shared_ptr<Pass::SpecializationData>> &Pass::getSpecilizationDatas() const
	{
		return m_mapSpecilizationDatas;
	}

	std::vector<vk::PushConstantRange> Pass::getPushConstantRanges() const
	{
		size_t size = m_arrPushConstantRangeNames.size();
		std::vector<vk::PushConstantRange> pushConstantRanges(size);
		for (size_t i = 0; i < size; ++i)
		{
			const auto &name = m_arrPushConstantRangeNames[i];
			const auto &iterator = m_mapPushConstantRanges.find(name);
			pushConstantRanges[i] = iterator->second;
		}
		return pushConstantRanges;
	}

    std::vector<std::shared_ptr<Pass::PushConstantUpdate>> Pass::getPushconstantUpdates() const
	{
		size_t size = m_arrPushConstantUpdateNames.size();
		std::vector<std::shared_ptr<Pass::PushConstantUpdate>> pPushConstantUpdates(size);
		for (size_t i = 0; i < size; ++i)
		{
			const auto &name = m_arrPushConstantUpdateNames[i];
			const auto &iterator = m_mapPPushConstantUpdates.find(name);
			pPushConstantUpdates[i] = iterator->second;
		}
		return pPushConstantUpdates;
	}

	void Pass::setSpecializationData(vk::ShaderStageFlagBits shaderStage
		, const vk::SpecializationInfo &info)
	{
		std::shared_ptr<SpecializationData> pSpecializationData = m_mapSpecilizationDatas[shaderStage];
		if (pSpecializationData == nullptr) 
		{
			pSpecializationData = std::shared_ptr<SpecializationData>(new SpecializationData());
			m_mapSpecilizationDatas[shaderStage] = pSpecializationData;
		}
		pSpecializationData->init(info);
		_updatePipelineStateID();
	}

	void Pass::setPushConstantRange(std::string name
		, vk::ShaderStageFlags stageFlags
		, uint32_t offset
		, uint32_t size)
	{
		vk::PushConstantRange pushConstantRange(stageFlags, offset, size);
		setValue(name, pushConstantRange, m_mapPushConstantRanges, m_arrPushConstantRangeNames);
		m_pipelineLayoutChanged = VG_TRUE;
	}

	void Pass::setPushConstantUpdate(std::string name
		, const void *pData
		, uint32_t size
		, vk::ShaderStageFlags stageFlags
		, uint32_t offset)
	{
		std::shared_ptr<PushConstantUpdate> pPushConstantUpdate(new PushConstantUpdate());
		pPushConstantUpdate->init(pData, size, stageFlags, offset);
		setValue(name, pPushConstantUpdate, m_mapPPushConstantUpdates, m_arrPushConstantUpdateNames);
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

	uint32_t Pass::getExtUniformBufferCount() const
	{
		return m_extUniformBufferCount;
	}

	const Pass::ExternalUniformBufferInfo * Pass::getExtUniformBuffers() const
	{
		return m_extUniformBuffers.data();
	}

	void Pass::setExtUniformBufferCount(uint32_t value)
	{
		m_extUniformBufferCount = value;
		m_extUniformBuffers.resize(value);
	}

	void Pass::setExtUniformBuffers(fd::ArrayProxy<ExternalUniformBufferInfo> extUniformBuffers
		, uint32_t offset
		)
	{
		uint32_t count = static_cast<uint32_t>(extUniformBuffers.size());
		uint32_t total = m_extUniformBufferCount;
		for (uint32_t i = 0; i < count && offset < total; ++i, ++offset) {
			m_extUniformBuffers[offset] = *(extUniformBuffers.data() + i);
		}
		m_descriptorSetsChanged = VG_TRUE;
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
		if (m_applied == VG_FALSE)
		{
			_createPipelineLayout();
			_createUniformBuffer();
			_createDescriptorSet();
			_beginCheckNeedUpdateDescriptorInfo();
			if (m_needUpdateDescriptorInfo == VG_TRUE) {
			    _updateDescriptorBufferInfo();
			    _updateDescriptorImageInfo();
			}
			if (m_needUpdateDescriptorInfo == VG_TRUE ||
			    m_textureChanged == VG_TRUE) {
				_updateDescriptorImageInfo();
				m_textureChanged = VG_FALSE;
			}
			_endCheckNeedUpdateDescriptorInfo();
			_applyUniformBufferDynamicOffsets();
			m_applied = VG_TRUE;
		}
		if (m_dataChanged == VG_TRUE)
		{
			_applyBufferContent();
			m_dataChanged = VG_FALSE;
		}
		
	}

	void Pass::beginRecord() const
	{
		if (m_applied == VG_FALSE) throw std::runtime_error("Pass should apply change before used to render.");
	}
		
	void Pass::endRecord() const
	{

	}

	void Pass::_createPipelineLayout()
	{
		m_lastLayoutBindingInfos.clear();
		for (const auto& name : m_arrLayoutBindNames)
		{
			const auto& item = m_mapLayoutBinds[name];
			m_lastLayoutBindingInfos.insert(item);
		}

		std::vector<vk::DescriptorSetLayoutBinding> bindings(m_arrLayoutBindNames.size());
		size_t index = 0;
		for (const auto& item : m_lastLayoutBindingInfos)
		{
			bindings[index].binding = index;
			bindings[index].descriptorCount = item.descriptorCount;
			bindings[index].descriptorType = tranDescriptorTypeToVK(item.descriptorType);
			bindings[index].stageFlags = tranShaderStageFlagsToVK(item.stageFlags);
			++index;

		}

		Bool32 bindingLayoutChanged = VG_FALSE;
		if (m_lastBindings != bindings)
		{
			bindingLayoutChanged = VG_TRUE;
		}

		if (bindingLayoutChanged)
		{
			auto pDevice = pApp->getDevice();
			//create descriptor set layout.
			if (bindings.size())
			{
				vk::DescriptorSetLayoutCreateInfo createInfo =
				{
					vk::DescriptorSetLayoutCreateFlags(),
					static_cast<uint32_t>(bindings.size()),
					bindings.data()
				};

				m_pDescriptorSetLayout = fd::createDescriptorSetLayout(pDevice, createInfo);
			}
			else
			{
				m_pDescriptorSetLayout = nullptr;
			}
			m_lastBindings = bindings;
			m_needReAllocateDescriptorSet = VG_TRUE;
		}

        //push constants
		auto pushConstantRanges = getPushConstantRanges();
		Bool32 pushConstantRangesChanged = VG_FALSE;
		if (m_lastPushConstantRanges != pushConstantRanges) {
			pushConstantRangesChanged = VG_TRUE;
			m_lastPushConstantRanges = pushConstantRanges;
		}

		auto pLayout = m_pDescriptorSetLayout;
		uint32_t layoutCount = pLayout != nullptr ? 1 : 0;
		for (const auto &extUniformbuffer : m_extUniformBuffers)
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
		m_usingDescriptorSets.resize(layoutCount);
		uint32_t layoutIndex = 0u;
		if (pLayout != nullptr) {
			setLayouts[layoutIndex] = *pLayout;
			if (m_needReAllocateDescriptorSet)
			{
				//now descriptor set has not be created.
				m_usingDescriptorSets[layoutIndex] = vk::DescriptorSet();
			}
			else
			{
				m_usingDescriptorSets[layoutIndex] = *m_pDescriptorSet;
			}
			++layoutIndex;
		}

		for (const auto &extUniformbuffer : m_extUniformBuffers)
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
		    		m_usingDescriptorSets[layoutIndex] = *(subData.getDescriptorSet());
		    		++layoutIndex;
		    	}
		    }
		}

		Bool32 setLayoutChanged = VG_FALSE;
		if (m_lastsetLayouts != setLayouts)
		{
			setLayoutChanged = VG_TRUE;
			m_lastsetLayouts = setLayouts;
		}


		if (bindingLayoutChanged || pushConstantRangesChanged || setLayoutChanged)
		{
			vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
				vk::PipelineLayoutCreateFlags(),             //flags
				layoutCount,                                 //setLayoutCount
				setLayouts.data(),                           //pSetLayouts
				pushConstantRanges.size(),                   //pushConstantRangeCount
				pushConstantRanges.data()                    //pPushConstantRanges
			};

			auto pDevice = pApp->getDevice();
			m_pPipelineLayout = fd::createPipelineLayout(pDevice, pipelineLayoutCreateInfo);
			_updatePipelineStateID();
		}
	}

	void Pass::_createUniformBuffer()
	{
		//get total size of uniform buffer datas and their offsets and sizes for each one.
		uint32_t totalSize = 0u;
		for (const auto& name : m_arrLayoutBindNames)
		{
			const auto& item = m_mapLayoutBinds[name];
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
			{
				totalSize += item.bufferSize;
			}
		}
		if (totalSize > m_uniformBufferSize)
		{
			createBuffer(static_cast<vk::DeviceSize>(totalSize), m_pUniformBuffer, m_pUniformBufferMemory);
			m_uniformBufferSize = totalSize;
		}
		// else
		// {
		// 	m_pUniformBuffer = nullptr;
		// 	m_pUniformBufferMemory = nullptr;
		// }
	}

	void Pass::_createDescriptorSet()
	{
		std::unordered_map<vk::DescriptorType, uint32_t> mapTypeCounts;
		for (const auto& name : m_arrLayoutBindNames)
		{
			const auto& item = m_mapLayoutBinds[name];
			vk::DescriptorType vkDescriptorType = tranDescriptorTypeToVK(item.descriptorType);
			mapTypeCounts[vkDescriptorType] += item.descriptorCount; //??? + 1.
		}

		std::unordered_map<vk::DescriptorType, uint32_t> mapAllTypes;

		for (const auto &pair : mapTypeCounts) {
			mapAllTypes[pair.first] = 1u;
		}

		for (const auto &pair : m_lastPoolSizeInfos) {
			mapAllTypes[pair.first] = 1u;
		}

		Bool32 isSame = VG_TRUE;
		for (const auto &pair : mapAllTypes) {
			if (m_lastPoolSizeInfos[pair.first] != mapTypeCounts[pair.first]) {
				isSame = VG_FALSE;
				break;
			}
		}

		//Save old descriptor pool because it will be used to destory the descriptorset later.
		std::shared_ptr<vk::DescriptorPool> oldPool = m_pDescriptorPool;
		if (isSame == VG_FALSE)
		{
			m_needReAllocateDescriptorSet = VG_TRUE;
			m_lastPoolSizeInfos = mapTypeCounts;

			std::vector<vk::DescriptorPoolSize> poolSizeInfos(mapTypeCounts.size());
		    size_t index = 0;
		    for (const auto& item : mapTypeCounts)
		    {
		    	poolSizeInfos[index].type = item.first;
		    	poolSizeInfos[index].descriptorCount = item.second;
		    	++index;
		    }

			auto pDevice = pApp->getDevice();
		    //create descriptor pool.
		    {
		    	if (poolSizeInfos.size())
		    	{
		    		vk::DescriptorPoolCreateInfo createInfo = { vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet
		    			, 1u
		    			, static_cast<uint32_t>(poolSizeInfos.size())
		    			, poolSizeInfos.data()
		    		};
		    		m_pDescriptorPool = fd::createDescriptorPool(pDevice, createInfo);
		    	}
		    	else
		    	{
		    		m_pDescriptorPool = nullptr;
		    	}
		    }
		}

		if (m_needReAllocateDescriptorSet) {
			m_needReAllocateDescriptorSet = VG_FALSE;
			//We must update descriptor information after reallocating descriptor set.
			m_needUpdateDescriptorInfo = VG_TRUE; 
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
		    	m_pDescriptorSet = fd::allocateDescriptorSet(pDevice, m_pDescriptorPool.get(), allocateInfo);
				m_usingDescriptorSets[0] = *m_pDescriptorSet;
		    }
		    else
		    {
		    	m_pDescriptorSet = nullptr;
		    }
		}
		
	}

	void Pass::_beginCheckNeedUpdateDescriptorInfo()
	{
		if (m_needUpdateDescriptorInfo == VG_TRUE) return;
		if (m_arrLayoutBindNames != m_lastLayoutBindNames) {
			m_needUpdateDescriptorInfo = VG_TRUE;
			return;
		}
		for (const auto &name : m_arrLayoutBindNames) {
			const auto &value1 = m_mapLayoutBinds[name];
			const auto &value2 = m_lastLayoutBinds[name];
			if (value1 != value2) {
				m_needUpdateDescriptorInfo = VG_TRUE;
				return;
			}
		}
	}

	void Pass::_updateDescriptorBufferInfo()
	{
		//get total number of unimform buffer variables.
		int32_t count = 0u;
		for (const auto& item : m_lastLayoutBindingInfos)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
				++count;
		}

		std::vector<vk::WriteDescriptorSet> writes(count);
		std::vector<std::vector<vk::DescriptorBufferInfo>> bufferInfoss(count);
		uint32_t offset = 0u;
		uint32_t index = 0u;
		uint32_t itemIndex = 0;
		for (const auto& item : m_lastLayoutBindingInfos)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
			{
				bufferInfoss[index].resize(item.descriptorCount);
				for (uint32_t i = 0; i < item.descriptorCount; ++i)
				{
					bufferInfoss[index][i].buffer = *m_pUniformBuffer;
					bufferInfoss[index][i].offset = offset;
					bufferInfoss[index][i].range = item.size;
					offset += item.bufferSize;
				}
				writes[index].dstSet = *m_pDescriptorSet;
				writes[index].dstBinding = itemIndex;
				writes[index].descriptorType = tranDescriptorTypeToVK(item.descriptorType);
				writes[index].dstArrayElement = 0;
				writes[index].descriptorCount = item.descriptorCount;
				writes[index].pBufferInfo = bufferInfoss[index].data();
				++index;
			}
			++itemIndex;
		}

		if (writes.size())
		{
			auto pDevice = pApp->getDevice();
			pDevice->updateDescriptorSets(writes, nullptr);
		}
	}

	void Pass::_updateDescriptorImageInfo()
	{
		//get total number of unimform buffer variables.
		int32_t count = 0u;
		for (const auto& item : m_lastLayoutBindingInfos)
		{
			if (item.descriptorType == DescriptorType::COMBINED_IMAGE_SAMPLER ||
				item.descriptorType == DescriptorType::INPUT_ATTACHMENT)
				++count;
		}

		std::vector<vk::WriteDescriptorSet> writes(count);
		std::vector<vk::DescriptorImageInfo> imageInfos(count);

		uint32_t index = 0u;
		uint32_t itemIndex = 0u;
		for (const auto& item : m_lastLayoutBindingInfos)
		{
			if (item.descriptorType == DescriptorType::COMBINED_IMAGE_SAMPLER ||
				item.descriptorType == DescriptorType::INPUT_ATTACHMENT)
			{
#ifdef DEBUG
				if (item.isTexture == VG_FALSE)
					throw std::runtime_error("The data type of binding should be is TEXTURE when its type is COMBINED_IMAGE_SAMPLER");
				if (item.descriptorCount != 1u)
					throw std::runtime_error("The descriptor count of binding shoubld be 1 when its type is COMBINED_IMAGE_SAMPLER");
#endif // DEBUG
				PassData::TexData texData = m_pData->getTexture(item.name);
				const auto pTexture = texData.pTexture != nullptr ? texData.pTexture : pDefaultTexture2D.get();
				vk::ImageView imageView;
				if (texData.pTexture != nullptr) {
					if (texData.pImageView != nullptr) {
					    imageView = *(texData.pImageView->getImageView());
					} else {
						imageView = *(pTexture->getImageView()->getImageView());
					}
				} else {
					imageView = *(pTexture->getImageView()->getImageView());
				}
				vk::Sampler sampler;
				if (texData.pSampler != nullptr) {
				    sampler = *(texData.pSampler->getSampler());
				} else {
					sampler = *(pTexture->getSampler()->getSampler());
				}
				vk::ImageLayout imageLayout;
				if (texData.imageLayout != vk::ImageLayout::eUndefined) {
					imageLayout = texData.imageLayout;
				} else {
					imageLayout = pTexture->getImage()->getInfo().layout;
				}
				
				imageInfos[index].sampler = sampler;
				imageInfos[index].imageView = imageView;
				imageInfos[index].imageLayout = imageLayout;

				writes[index].dstSet = *m_pDescriptorSet;
				writes[index].dstBinding = itemIndex;
				writes[index].descriptorType = tranDescriptorTypeToVK(item.descriptorType);
				writes[index].dstArrayElement = 0u;
				writes[index].descriptorCount = item.descriptorCount;
				writes[index].pImageInfo = &imageInfos[index];
				++index;
			}
			++itemIndex;
		}

		auto pDevice = pApp->getDevice();
		pDevice->updateDescriptorSets(writes, nullptr);
	}

	void Pass::_endCheckNeedUpdateDescriptorInfo()
	{
		if (m_needUpdateDescriptorInfo == VG_TRUE) 
		{
			m_lastLayoutBindNames = m_arrLayoutBindNames;
			m_lastLayoutBinds = m_mapLayoutBinds;
		}
		m_needUpdateDescriptorInfo = VG_FALSE;
		
	}

	void Pass::_applyBufferContent()
	{
		if (m_pUniformBufferMemory != nullptr)
		{
			//get total number of unimform buffer variables.
			int32_t uniformBufferCount = 0u;
			for (const auto& name : m_arrLayoutBindNames)
			{
				const auto& item = m_mapLayoutBinds[name];
				if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
					++uniformBufferCount;
			}

			//get total size of uniform buffer datas and their offsets and sizes for each one.
			uint32_t totalSize = 0u;
			uint32_t offset = 0u;
			std::vector<uint32_t> offsets(uniformBufferCount);
			std::vector<std::string> names(uniformBufferCount);
			std::vector<uint32_t> descriptorCounts(uniformBufferCount);
			uint32_t index = 0u;
			for (const auto& name : m_arrLayoutBindNames)
			{
				const auto& item = m_mapLayoutBinds[name];
				if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
				{
					offsets[index] = offset;
					names[index] = item.name;
					descriptorCounts[index] = item.descriptorCount;
					totalSize += item.bufferSize;
					offset += item.bufferSize;
					++index;
				}
			}
			//sync buffer data.
			void *data;
			auto pDevice = pApp->getDevice();
			pDevice->mapMemory(*m_pUniformBufferMemory, 0, static_cast<vk::DeviceSize>(totalSize), vk::MemoryMapFlags(), &data);
			for (int32_t i = 0; i < uniformBufferCount; ++i)
			{
				m_pData->memoryCopyData(names[i], data, offsets[i], 0u, descriptorCounts[i]);
			}
			pDevice->unmapMemory(*m_pUniformBufferMemory);
		}
	}

	void Pass::createBuffer(vk::DeviceSize size, std::shared_ptr<vk::Buffer>& pBuffer, std::shared_ptr<vk::DeviceMemory> &pBufferMemory)
	{
		vk::BufferCreateInfo createInfo = {
			vk::BufferCreateFlags(),
			size,
			vk::BufferUsageFlagBits::eUniformBuffer,
			vk::SharingMode::eExclusive
		};

		auto pDevice = pApp->getDevice();
		pBuffer = fd::createBuffer(pDevice, createInfo);

		vk::MemoryRequirements memReqs = pDevice->getBufferMemoryRequirements(*pBuffer);
		vk::MemoryAllocateInfo allocateInfo = {
			memReqs.size,
			vg::findMemoryType(pApp->getPhysicalDevice(),
			memReqs.memoryTypeBits,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
		};

		pBufferMemory = fd::allocateMemory(pDevice, allocateInfo);

		pDevice->bindBufferMemory(*pBuffer, *pBufferMemory, 0);
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
		for (const auto &extUniformbuffer : m_extUniformBuffers)
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

		if (static_cast<uint32_t>(m_usingDynamicOffsets.size()) != dynamicCount)
		      m_usingDynamicOffsets.resize(dynamicCount);
		uint32_t dynamicIndex = 0u;

		for (const auto &extUniformbuffer : m_extUniformBuffers)
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
		    					if(m_usingDynamicOffsets[dynamicIndex] != descriptorInfo.dynamicOffset)
		    					    m_usingDynamicOffsets[dynamicIndex] = descriptorInfo.dynamicOffset;
		    					++dynamicIndex;
		    				}
		    			}
		    			descriptorInfoIndex += binding.descriptorCount;
		    		}
    
		    	}
		    }
		}
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
		m_pData->setDataValue(VG_M_BUILDIN_NAME, nullptr, size, 0u);
		
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
					m_pData->setDataValue(VG_M_BUILDIN_NAME
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

		if (size > 0) {
		    //update layout binding information.
		    uint32_t descriptorCount = 1u;
		    LayoutBindingInfo info(
		    	VG_M_BUILDIN_NAME,
		    	VG_FALSE,
		    	VG_M_BUILDIN_BINDING_PRIORITY,
		    	DescriptorType::UNIFORM_BUFFER,
		    	descriptorCount,
		    	ShaderStageFlagBits::VERTEX
		    );
		    info.updateSize(size);
		    setValue(VG_M_BUILDIN_NAME, info, m_mapLayoutBinds, m_arrLayoutBindNames);
		} else {
			removeValue(VG_M_BUILDIN_NAME, m_mapLayoutBinds, m_arrLayoutBindNames);
		}
		m_applied = VG_FALSE;
		m_dataChanged = VG_TRUE;
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
}