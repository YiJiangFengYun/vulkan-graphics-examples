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

	Pass::Pass() 
		: Base(BaseType::PASS)
		, m_data()
		, m_dataChanged(VG_FALSE)
		, m_dataContentChanged(VG_FALSE)
		, m_dataContentChanges()
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
		, m_descriptorSetsChanged(VG_FALSE)
		, m_descriptorSetLayouts()
		, m_descriptorSets()
		, m_dynamicOffsetsChanged(VG_FALSE)
		, m_dynamicOffsets()
		, m_pushConstantChanged(VG_FALSE)
		, m_pushConstantRanges()
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
		return m_data.getBuffer(name);
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
		return m_data.getTexture(name);
	}
	
	void Pass::setTexture(std::string name, const PassTextureInfo &texInfo)
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

	Bool32 Pass::hasPushConstantRange(std::string name) const
	{
		return hasValue(name, m_mapPushConstantRanges);
	}
		
	void Pass::addPushConstantRange(std::string name, vk::ShaderStageFlags stageFlags
		, uint32_t offset
		, uint32_t size
		)
	{
		vk::PushConstantRange pushConstantRange(stageFlags, offset, size);
		addValue(name, pushConstantRange, m_mapPushConstantRanges, m_arrPushConstantRangeNames);
		m_pushConstantChanged = VG_TRUE;
	}

	void Pass::removePushConstantRange(std::string name)
	{
		removeValue(name, m_mapPushConstantRanges, m_arrPushConstantRangeNames);
	}

	void Pass::setPushConstantRange(std::string name
		, vk::ShaderStageFlags stageFlags
		, uint32_t offset
		, uint32_t size)
	{
		vk::PushConstantRange pushConstantRange(stageFlags, offset, size);
		setValue(name, pushConstantRange, m_mapPushConstantRanges, m_arrPushConstantRangeNames);
		m_pushConstantChanged = VG_TRUE;
	}

	Bool32 Pass::hasPushConstantUpdate(std::string name) const
	{
		return hasValue(name, m_mapPPushConstantUpdates);
	}

	void Pass::addPushConstantUpdate(std::string name
	    , const void *pData
		, uint32_t size
		, vk::ShaderStageFlags stageFlags
		, uint32_t offset
		)
	{
		std::shared_ptr<PushConstantUpdate> pPushConstantUpdate(new PushConstantUpdate());
		pPushConstantUpdate->init(pData, size, stageFlags, offset);
		addValue(name, pPushConstantUpdate, m_mapPPushConstantUpdates, m_arrPushConstantUpdateNames);
	}

	void Pass::removePushConstantUpdate(std::string name)
	{
		removeValue(name, m_mapPPushConstantUpdates, m_arrPushConstantUpdateNames);
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
		m_descriptorSetsChanged = VG_TRUE;
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
				const auto &bufferInfo = data.getBuffer(name);
				BufferTextureSortInfo sortInfo = {
					name,
					bufferInfo.bindingPriority,
					VG_FALSE,
					reinterpret_cast<const void *>(&bufferInfo),
				};
				sortInfos.insert(sortInfo);
			}
			const auto &arrTextureNames = data.arrTexNames;
			for (const auto &name : arrTextureNames)
			{
				const auto &textureInfo = data.getTexture(name);
				BufferTextureSortInfo sortInfo = {
					name,
					textureInfo.bindingPriority,
					VG_TRUE,
					reinterpret_cast<const void *>(&textureInfo),
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
						const auto &textureInfo = *(reinterpret_cast<const PassTextureInfo *>(sortInfo.pInfo));
						bindingInfo.descriptorType = tranImageDescriptorTypeToVK(textureInfo.descriptorType);
						bindingInfo.descriptorCount = 1u;
						bindingInfo.stageFlags = textureInfo.stageFlags;

				        const auto pTexture = textureInfo.pTexture != nullptr ? textureInfo.pTexture : pDefaultTexture2D.get();
				        vk::ImageView imageView;
				        if (textureInfo.pTexture != nullptr) {
				        	if (textureInfo.pImageView != nullptr) {
				        	    imageView = *(textureInfo.pImageView->getImageView());
				        	} else {
				        		imageView = *(pTexture->getImageView()->getImageView());
				        	}
				        } else {
				        	imageView = *(pTexture->getImageView()->getImageView());
				        }
				        vk::Sampler sampler;
				        if (textureInfo.pSampler != nullptr) {
				            sampler = *(textureInfo.pSampler->getSampler());
				        } else {
				        	sampler = *(pTexture->getSampler()->getSampler());
				        }
				        vk::ImageLayout imageLayout;
				        if (textureInfo.imageLayout != vk::ImageLayout::eUndefined) {
				        	imageLayout = textureInfo.imageLayout;
				        } else {
				        	imageLayout = pTexture->getImage()->getInfo().layout;
				        }

						updateDesSetInfo.imageInfos.resize(1u);
						updateDesSetInfo.imageInfos[0].imageView = imageView;
						updateDesSetInfo.imageInfos[0].sampler = sampler;
						updateDesSetInfo.imageInfos[0].imageLayout = imageLayout;
					}
					else
					{
						const auto &bufferInfo = *(reinterpret_cast<const PassBufferInfo *>(sortInfo.pInfo));
						bindingInfo.descriptorType = tranBufferDescriptorTypeToVK(bufferInfo.descriptorType);
						bindingInfo.descriptorCount = 1u;
						bindingInfo.stageFlags = bufferInfo.stageFlags;

						updateDesSetInfo.bufferInfos.resize(1u);
					    updateDesSetInfo.bufferInfos[0].buffer = *(bufferInfo.pBuffer->getBuffer());
					    updateDesSetInfo.bufferInfos[0].offset = bufferInfo.offset;
						updateDesSetInfo.bufferInfos[0].range = bufferInfo.range;
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

		//Update descriptor sets and their layouts.
		if (m_descriptorSetsChanged) {
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
		    std::vector<vk::DescriptorSet> descriptorSets(layoutCount);
		    uint32_t layoutIndex = 0u;
		    if (pLayout != nullptr) {
		    	setLayouts[layoutIndex] = *pLayout;
		    	descriptorSets[layoutIndex] = *m_pDescriptorSet;
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
		    auto pushConstantRanges = getPushConstantRanges();
		    if (m_pushConstantRanges != pushConstantRanges) {
		    	m_pushConstantRanges = pushConstantRanges;
				//Push constant change will make pipeline layout change.
				m_pipelineLayoutChanged = VG_TRUE;
		    }

			m_pushConstantChanged = VG_FALSE;
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
		if (m_dataChanged || m_dataContentChanged || m_textureChanged || m_bufferChanged || 
		    m_descriptorSetChanged || m_descriptorSetsChanged || m_dynamicOffsetsChanged || m_pipelineLayoutChanged)
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

		if (static_cast<uint32_t>(m_dynamicOffsets.size()) != dynamicCount)
		      m_dynamicOffsets.resize(dynamicCount);
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
		, const void *pInfo
		)
		: name(name)
		, bindingPriority(bindingPriority)		
		, isTexture(isTexture)
		, pInfo(pInfo)
	{

	}

	Pass::UpdateDescriptorSetInfo::UpdateDescriptorSetInfo(std::vector<vk::DescriptorBufferInfo> bufferInfos
		, std::vector<vk::DescriptorImageInfo> imageInfos
	    )
		:  bufferInfos(bufferInfos)
		, imageInfos(imageInfos)
	{

	}
}