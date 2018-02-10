#include "graphics/material/pass.hpp"

namespace vg
{
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
		init(target.m_pData, target.m_size, target.m_info);
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
		init(target.m_pData, target.m_size, target.m_info);
		return *this;
	}

	void Pass::SpecializationData::init(const void* pData
		, uint32_t size
		, const vk::SpecializationInfo &info)
	{
		uint32_t count = info.mapEntryCount;
		size_t sizeOfSrcMap = count * sizeof(vk::SpecializationMapEntry);
		m_info.mapEntryCount = count;
		m_mapEntries.resize(count);
		memcpy(m_mapEntries.data(), info.pMapEntries, sizeOfSrcMap);
        m_info.pMapEntries = m_mapEntries.data();
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
		m_info.dataSize = size;
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

	Pass::LayoutBindingInfo::LayoutBindingInfo()
	{
	}

	Pass::LayoutBindingInfo::LayoutBindingInfo(std::string name
		, Bool32 isTexture
		, std::uint32_t binding
		, DescriptorType descriptorType
		, std::uint32_t descriptorCount
		, ShaderStageFlags stageFlags
	) 
		: name(name)
		, isTexture(isTexture)
		, binding(binding)
		, descriptorType(descriptorType)
		, descriptorCount(descriptorCount)
		, stageFlags(stageFlags)
	{
	}

	Pass::LayoutBindingInfo::LayoutBindingInfo(const LayoutBindingInfo & target)
		: name(target.name)
		, isTexture(target.isTexture)
		, binding(target.binding)
		, descriptorType(target.descriptorType)
		, descriptorCount(target.descriptorCount)
		, stageFlags(target.stageFlags)
		, size(target.size)
		, bufferSize(target.bufferSize)
	{

	}

	Pass::LayoutBindingInfo& Pass::LayoutBindingInfo::operator=(const LayoutBindingInfo &target)
	{
		name = target.name;
		isTexture = target.isTexture;
		binding = target.binding;
		descriptorType = target.descriptorType;
		descriptorCount = target.descriptorCount;
		stageFlags = target.stageFlags;
		size = target.size;
		bufferSize = target.bufferSize;

		return *this;
	}

	Pass::LayoutBindingInfo::LayoutBindingInfo(const LayoutBindingInfo &&target)
		: name(target.name)
		, isTexture(target.isTexture)
		, binding(target.binding)
		, descriptorType(target.descriptorType)
		, descriptorCount(target.descriptorCount)
		, stageFlags(target.stageFlags)
		, size(target.size)
		, bufferSize(target.bufferSize)
	{

	}

	void Pass::LayoutBindingInfo::updateSize(const std::shared_ptr<MaterialData> &pMaterialData)
	{
		if (isTexture)
		{
			size = 0u;
			bufferSize = 0u;
			return;
		}
		auto pPhysicalDevice = pApp->getPhysicalDevice();
		auto properties = pPhysicalDevice->getProperties();
		auto minOffsetAlignment = static_cast<float>(properties.limits.minUniformBufferOffsetAlignment);
		size = pMaterialData->getDataBaseSize(name) * descriptorCount;
		bufferSize = static_cast<uint32_t>(std::ceil(size / minOffsetAlignment) * minOffsetAlignment);
	}

	Bool32 Pass::LayoutBindingInfo::operator ==(const LayoutBindingInfo& target) const
	{
		return name == target.name 
			&& isTexture == target.isTexture 
			&& binding == target.binding 
			&& descriptorType == target.descriptorType 
			&& descriptorCount == target.descriptorCount 
			&& stageFlags == target.stageFlags;
	}

	Pass::Pass() 
		: Base(BaseType::PASS)
		, m_pData(new MaterialData())
		, m_applied(VG_FALSE)
		, m_cullMode(CullModeFlagBits::eBack)
		, m_frontFace(FrontFaceType::eCounterClockwise)
		, m_viewport(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f)
		, m_scissor(0.0f, 0.0f, 1.0f, 1.0f)
		, m_depthStencilInfo()
		, m_colorBlendInfo()
		, m_mapSpecilizationDatas()
		, m_mapPushConstantRanges()
		, m_arrPushConstantRangeNames()
		, m_mapPPushConstantUpdates()		
		, m_arrPushConstantUpdateNames()
		, m_buildInData()
		, m_pipelineStateID()
		, m_lastBindings()
	{
	}

	Pass::Pass(std::shared_ptr<Shader> pShader)
		: Base(BaseType::PASS)
		, m_pData(new MaterialData())
		, m_applied(VG_FALSE)
		, m_pShader(pShader)
		, m_cullMode(CullModeFlagBits::eBack)
		, m_frontFace(FrontFaceType::eCounterClockwise)
		, m_viewport(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f)
		, m_scissor(0.0f, 0.0f, 1.0f, 1.0f)
		, m_depthStencilInfo()
		, m_colorBlendInfo()
		, m_mapSpecilizationDatas()
		, m_mapPushConstantRanges()
		, m_arrPushConstantRangeNames()
		, m_mapPPushConstantUpdates()		
		, m_arrPushConstantUpdateNames()
		, m_buildInData()
		, m_pipelineStateID()
		, m_lastBindings()
	{

	}


	Pass::~Pass()
	{
	}

	std::shared_ptr<Shader> Pass::getShader()
	{
		return m_pShader;
	}

	void Pass::setShader(std::shared_ptr<Shader> pShader)
	{
		m_pShader = pShader;
		_updatePipelineStateID();
	}

	const std::shared_ptr<Texture> &Pass::getTexture(std::string name) const
	{
		return m_pData->getTexture(name);
	}
	void Pass::setTexture(std::string name
		, const std::shared_ptr<Texture> &pTex
		, uint32_t binding
		, DescriptorType descriptorType
		, ShaderStageFlags stageFlags
	)
	{
		m_pData->setTexture(name, pTex);
		//update layout binding information.
		uint32_t descriptorCount = 1u;
		LayoutBindingInfo info(
			name,
			VG_TRUE,
			binding,
			descriptorType,
			descriptorCount,
			stageFlags
		);
		info.updateSize(m_pData);
		setValue(name, info, m_mapLayoutBinds, m_arrLayoutBindNames);
		m_applied = VG_FALSE;
	}

	const std::shared_ptr<Texture> &Pass::getMainTexture() const
	{
		return getTexture(VG_M_MAIN_TEXTURE_NAME);
	}

	void Pass::setMainTexture(const std::shared_ptr<Texture> value)
	{
		setTexture(VG_M_MAIN_TEXTURE_NAME, value, VG_M_MAIN_TEXTURE_BINDING,
			DescriptorType::COMBINED_IMAGE_SAMPLER, ShaderStageFlagBits::FRAGMENT);
	}

	Color Pass::getMainColor() const
	{
		return m_buildInData.mainColor;
	}

	void Pass::setMainColor(Color color)
	{
		m_buildInData.mainColor = color;
		setDataValue(VG_M_BUILDIN_NAME
			, m_buildInData
			, VG_M_BUILDIN_BINDING
			, DescriptorType::UNIFORM_BUFFER
			, ShaderStageFlagBits::VERTEX
		);
	}

	void Pass::_setBuildInMatrixData(Matrix4x4 matrixObjectToNDC
		, Matrix4x4 matrixObjectToView
		, Matrix4x4 matrixObjectToWorld
	)
	{
		if (m_buildInData.matrixObjectToNDC == matrixObjectToNDC &&
			m_buildInData.matrixObjectToView == matrixObjectToView &&
			m_buildInData.matrixObjectToWorld == matrixObjectToWorld)
			return;
		m_buildInData.matrixObjectToNDC = matrixObjectToNDC;
		m_buildInData.matrixObjectToView = matrixObjectToView;
		m_buildInData.matrixObjectToWorld = matrixObjectToWorld;
		setDataValue(VG_M_BUILDIN_NAME
			, m_buildInData
			, VG_M_BUILDIN_BINDING
			, DescriptorType::UNIFORM_BUFFER
			, ShaderStageFlagBits::VERTEX
		);
	}

	void Pass::apply()
	{
		if (m_applied == VG_FALSE)
		{
			_createDescriptorSetLayout();
			_createPipelineLayout();
			_createUniformBuffer();
			_createDescriptorSet();
			_updateDescriptorBufferInfo();
			_updateDescriptorImageInfo();
			_applyBufferContent();
			m_applied = VG_TRUE;
		}
	}

	CullModeFlags Pass::getCullMode() const
	{
		return m_cullMode;
	}

	void Pass::setCullMode(CullModeFlags cullMode)
	{
		m_cullMode = cullMode;
		_updatePipelineStateID();
	}

	FrontFaceType Pass::getFrontFace() const
	{
		return m_frontFace;
	}

	void Pass::setFrontFace(FrontFaceType frontFace)
	{
		m_frontFace = frontFace;
		_updatePipelineStateID();
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
		else if (viewport.x > viewport.width)
			throw std::invalid_argument("The x of viewport is bigger than the width of viewport!");
		if (viewport.y < 0)
			throw std::invalid_argument("the y of viewport is smaller than 0!");
		else if(viewport.y > viewport.height)
			throw std::invalid_argument("The y of viewport is bigger than the height of viewport!");
		if(viewport.minDepth < 0)
			throw std::invalid_argument("the minDepth of viewport is smaller than 0!");
		else if(viewport.minDepth > viewport.maxDepth)
			throw std::invalid_argument("The minDepth of viewport is bigger than the maxDepth of viewport!");
#endif // DEBUG
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
		m_colorBlendInfo = value;
		_updatePipelineStateID();
	}

	const Bool32 Pass::IsHasSpecializationData(ShaderStageFlagBits shaderStage) const
	{
		auto vkStage = tranShaderStageFlagBitToVK(shaderStage);
		const auto& map = m_mapSpecilizationDatas;
		return map.count(vkStage) != 0;
	}

	const Bool32 Pass::IsHasSpecializationData(vk::ShaderStageFlagBits shaderStage) const
	{
		const auto& map = m_mapSpecilizationDatas;
		return map.count(shaderStage) != 0;
	}

	const std::shared_ptr<Pass::SpecializationData> &Pass::getSpecializationData(ShaderStageFlagBits shaderStage) const
	{
		auto vkStage = tranShaderStageFlagBitToVK(shaderStage);
		const auto& map = m_mapSpecilizationDatas;
		const auto& iterator = map.find(vkStage);
		if (iterator == map.cend())
		{
			throw std::runtime_error("SpecializationData don't exit.");
		}
		else
		{
			return iterator->second;
		}
	}

	const std::shared_ptr<Pass::SpecializationData> &Pass::getSpecializationData(vk::ShaderStageFlagBits shaderStage) const
	{
		const auto& map = m_mapSpecilizationDatas;
		const auto& iterator = map.find(shaderStage);
		if (iterator == map.cend())
		{
			throw std::runtime_error("SpecializationData don't exit.");
		}
		else
		{
			return iterator->second;
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

	Pass::PipelineStateID Pass::getPipelineStateID() const
    {
        return m_pipelineStateID;
    }

	void Pass::setSpecializationData(ShaderStageFlagBits shaderStage
		, void* pData
		, uint32_t size
		, const vk::SpecializationInfo &info)
	{
		auto vkStage = tranShaderStageFlagBitToVK(shaderStage);
		std::shared_ptr<SpecializationData> pSpecializationData = m_mapSpecilizationDatas[vkStage];
		if (pSpecializationData == nullptr) 
		{
			pSpecializationData = std::shared_ptr<SpecializationData>(new SpecializationData());
			m_mapSpecilizationDatas[vkStage] = pSpecializationData;
		}
		pSpecializationData->init(pData, size, info);
		_updatePipelineStateID();
	}

	void Pass::setPushConstantRange(std::string name
		, vk::ShaderStageFlags stageFlags
		, uint32_t offset
		, uint32_t size)
	{
		vk::PushConstantRange pushConstantRange(stageFlags, offset, size);
		setValue(name, pushConstantRange, m_mapPushConstantRanges, m_arrPushConstantRangeNames);
		m_applied = VG_FALSE;
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


	const std::shared_ptr<Shader> &Pass::getShader() const
	{
		return m_pShader;
	}

	const std::shared_ptr<vk::Buffer> &Pass::getUniformBuffer() const
	{
		return m_pUniformBuffer;
	}

	const std::shared_ptr<vk::DeviceMemory> &Pass::getUniformBufferMemory() const
	{
		return m_pUniformBufferMemory;
	}

	const std::shared_ptr<vk::DescriptorSetLayout> &Pass::getDescriptorSetLayout() const
	{
		return m_pDescriptorSetLayout;
	}

	const std::shared_ptr<vk::PipelineLayout> &Pass::getPipelineLayout() const
	{
		return m_pPipelineLayout;
	}

	const std::shared_ptr<vk::DescriptorPool> &Pass::getDescriptorPool() const
	{
		return m_pDescriptorPool;
	}

	const std::shared_ptr<vk::DescriptorSet> &Pass::getDescriptorSet() const
	{
		return m_pDescriptorSet;
	}

	void Pass::_createDescriptorSetLayout()
	{
		std::vector<vk::DescriptorSetLayoutBinding> bindings(m_arrLayoutBindNames.size());
		size_t index = 0;
		for (const auto& name : m_arrLayoutBindNames)
		{
			const auto& item = m_mapLayoutBinds[name];
			bindings[index].binding = item.binding;
			bindings[index].descriptorCount = item.descriptorCount;
			bindings[index].descriptorType = tranDescriptorTypeToVK(item.descriptorType);
			bindings[index].stageFlags = tranShaderStageFlagsToVK(item.stageFlags);
			++index;

		}

		if (m_lastBindings == bindings) return;

		auto pDevice = pApp->getDevice();

		//create descriptor set layout.
	    if(bindings.size())
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
		_updatePipelineStateID();
	}

	void Pass::_createPipelineLayout()
	{
		auto pLayout = m_pDescriptorSetLayout;
		uint32_t layoutCount = pLayout != nullptr ? 1 : 0;
		auto pSetLayouts = pLayout != nullptr ? pLayout.get() : nullptr;
		
		auto pushConstantRanges = getPushConstantRanges();

		vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
			vk::PipelineLayoutCreateFlags(),             //flags
			layoutCount,                                 //setLayoutCount
			pSetLayouts,                                 //pSetLayouts
			pushConstantRanges.size(),                   //pushConstantRangeCount
			pushConstantRanges.data()                    //pPushConstantRanges
		};

		auto pDevice = pApp->getDevice();
		m_pPipelineLayout = fd::createPipelineLayout(pDevice, pipelineLayoutCreateInfo);
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
		if (totalSize)
		{
			createBuffer(static_cast<vk::DeviceSize>(totalSize), m_pUniformBuffer, m_pUniformBufferMemory);
		}
		else
		{
			m_pUniformBuffer = nullptr;
			m_pUniformBufferMemory = nullptr;
		}
	}

	void Pass::_createDescriptorSet()
	{
		std::unordered_map<vk::DescriptorType, uint32_t> mapTypeCounts;
		size_t index = 0;
		for (const auto& name : m_arrLayoutBindNames)
		{
			const auto& item = m_mapLayoutBinds[name];
			vk::DescriptorType vkDescriptorType = tranDescriptorTypeToVK(item.descriptorType);
			mapTypeCounts[vkDescriptorType] += item.descriptorCount; //??? + 1.
		}

		auto pDevice = pApp->getDevice();
		//create descriptor pool.
		{
			std::vector<vk::DescriptorPoolSize> poolSizeInfos(mapTypeCounts.size());
			index = 0;
			for (const auto& item : mapTypeCounts)
			{
				poolSizeInfos[index].type = item.first;
				poolSizeInfos[index].descriptorCount = item.second;
				++index;
			}

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

		//create descriptor set.
		if (m_pDescriptorSetLayout != nullptr)
		{
			vk::DescriptorSetLayout layouts[] = { *m_pDescriptorSetLayout };
			vk::DescriptorSetAllocateInfo allocateInfo = {
				*m_pDescriptorPool,
				1u,
				layouts
			};
			m_pDescriptorSet = fd::allocateDescriptorSet(pDevice, m_pDescriptorPool, allocateInfo);
		}
		else
		{
			m_pDescriptorSet = nullptr;
		}
	}

	void Pass::_updateDescriptorBufferInfo()
	{
		//get total number of unimform buffer variables.
		int32_t count = 0u;
		for (const auto& name : m_arrLayoutBindNames)
		{
			const auto& item = m_mapLayoutBinds[name];
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
				++count;
		}

		std::vector<vk::WriteDescriptorSet> writes(count);
		std::vector<std::vector<vk::DescriptorBufferInfo>> bufferInfoss(count);
		uint32_t offset = 0u;
		uint32_t index = 0u;
		for (const auto& name : m_arrLayoutBindNames)
		{
			const auto& item = m_mapLayoutBinds[name];
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
				writes[index].dstBinding = item.binding;
				writes[index].descriptorType = tranDescriptorTypeToVK(item.descriptorType);
				writes[index].dstArrayElement = 0;
				writes[index].descriptorCount = item.descriptorCount;
				writes[index].pBufferInfo = bufferInfoss[index].data();
				++index;
			}
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
		for (const auto& name : m_arrLayoutBindNames)
		{
			const auto& item = m_mapLayoutBinds[name];
			if (item.descriptorType == DescriptorType::COMBINED_IMAGE_SAMPLER)
				++count;
		}

		std::vector<vk::WriteDescriptorSet> writes(count);
		std::vector<vk::DescriptorImageInfo> imageInfos(count);

		uint32_t index = 0u;
		for (const auto& name : m_arrLayoutBindNames)
		{
			const auto& item = m_mapLayoutBinds[name];
			if (item.descriptorType == DescriptorType::COMBINED_IMAGE_SAMPLER)
			{
#ifdef DEBUG
				if (item.isTexture == VG_FALSE)
					throw std::runtime_error("The data type of binding should be is TEXTURE when its type is COMBINED_IMAGE_SAMPLER");
				if (item.descriptorCount != 1u)
					throw std::runtime_error("The descriptor count of binding shoubld be 1 when its type is COMBINED_IMAGE_SAMPLER");
#endif // DEBUG
				std::shared_ptr<Texture> pTexture = getTexture(item.name);
				if (pTexture != nullptr)
				{
					imageInfos[index].sampler = *pTexture->_getSampler();
					imageInfos[index].imageView = *pTexture->_getImageView();
					imageInfos[index].imageLayout = pTexture->_getImageLayout();
				}

				writes[index].dstSet = *m_pDescriptorSet;
				writes[index].dstBinding = item.binding;
				writes[index].descriptorType = tranDescriptorTypeToVK(item.descriptorType);
				writes[index].dstArrayElement = 0u;
				writes[index].descriptorCount = item.descriptorCount;
				writes[index].pImageInfo = &imageInfos[index];
				++index;
			}
		}

		auto pDevice = pApp->getDevice();
		pDevice->updateDescriptorSets(writes, nullptr);
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
			m_pipelineStateID = 0;
		}
	}
}