#include "graphics/material/pass.hpp"

namespace kgs
{
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
		, m_applied(KGS_FALSE)
		, m_cullMode(CullModeFlagBits::eBack)
		, m_frontFace(FrontFaceType::eCounterClockwise)
		, m_viewport(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f)
		, m_scissor(0.0f, 0.0f, 1.0f, 1.0f)
		, m_depthStencilStateInfo()
		, m_colorBlendInfo()
		, m_buildInData()
	{
	}

	Pass::Pass(std::shared_ptr<Shader> pShader)
		: Base(BaseType::PASS)
		, m_pData(new MaterialData())
		, m_applied(KGS_FALSE)
		, m_pShader(pShader)
		, m_cullMode(CullModeFlagBits::eBack)
		, m_frontFace(FrontFaceType::eCounterClockwise)
		, m_viewport(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f)
		, m_scissor(0.0f, 0.0f, 1.0f, 1.0f)
		, m_depthStencilStateInfo()
		, m_colorBlendInfo()
		, m_buildInData()
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
			KGS_TRUE,
			binding,
			descriptorType,
			descriptorCount,
			stageFlags
		);
		info.updateSize(m_pData);
		setValue(name, info, m_mapLayoutBinds, m_arrLayoutBindNames);
		m_applied = KGS_FALSE;
	}

	const std::shared_ptr<Texture> &Pass::getMainTexture() const
	{
		return getTexture(KGS_M_MAIN_TEXTURE_NAME);
	}

	void Pass::setMainTexture(const std::shared_ptr<Texture> value)
	{
		setTexture(KGS_M_MAIN_TEXTURE_NAME, value, KGS_M_MAIN_TEXTURE_BINDING,
			DescriptorType::COMBINED_IMAGE_SAMPLER, ShaderStageFlagBits::FRAGMENT);
	}

	Color Pass::getMainColor() const
	{
		return m_buildInData.mainColor;
	}

	void Pass::setMainColor(Color color)
	{
		m_buildInData.mainColor = color;
		setDataValue(KGS_M_BUILDIN_NAME
			, m_buildInData
			, KGS_M_BUILDIN_BINDING
			, DescriptorType::UNIFORM_BUFFER
			, ShaderStageFlagBits::VERTEX
		);
	}

	void Pass::_setBuildInMatrixData(Matrix4x4 matrixObjectToNDC
		, Matrix4x4 matrixObjectToView
		, Matrix4x4 matrixObjectToWorld
	)
	{
		m_buildInData.matrixObjectToNDC = matrixObjectToNDC;
		m_buildInData.matrixObjectToView = matrixObjectToView;
		m_buildInData.matrixObjectToWorld = matrixObjectToWorld;
		setDataValue(KGS_M_BUILDIN_NAME
			, m_buildInData
			, KGS_M_BUILDIN_BINDING
			, DescriptorType::UNIFORM_BUFFER
			, ShaderStageFlagBits::VERTEX
		);
	}

	void Pass::apply()
	{
		if (m_applied == KGS_FALSE)
		{
			_createDescriptorSetLayout();
			_createUniformBuffer();
			_createDescriptorSet();
			_updateDescriptorBufferInfo();
			_updateDescriptorImageInfo();
			_applyBufferContent();
			m_applied = KGS_TRUE;
		}
	}

	CullModeFlags Pass::getCullMode() const
	{
		return m_cullMode;
	}

	void Pass::setCullMode(CullModeFlags cullMode)
	{
		m_cullMode = cullMode;
	}

	FrontFaceType Pass::getFrontFace() const
	{
		return m_frontFace;
	}

	void Pass::setFrontFace(FrontFaceType frontFace)
	{
		m_frontFace = frontFace;
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
	}

	const DepthStencilInfo &Pass::getDepthStencilStateInfo() const
	{
		return m_depthStencilStateInfo;
	}

	void Pass::setDepthStencilStateInfo(const DepthStencilInfo &value)
	{
		m_depthStencilStateInfo = value;
	}

	const ColorBlendInfo &Pass::getColorBlendInfo() const
	{
		return m_colorBlendInfo;
	}

	void Pass::setColorBlendInfo(const ColorBlendInfo &value)
	{
		m_colorBlendInfo = value;
	}


	std::shared_ptr<Shader> Pass::_getShader()
	{
		return m_pShader;
	}

	std::shared_ptr<vk::Buffer> Pass::_getUniformBuffer()
	{
		return m_pUniformBuffer;
	}

	std::shared_ptr<vk::DeviceMemory> Pass::_getUniformBufferMemory()
	{
		return m_pUniformBufferMemory;
	}

	std::shared_ptr<vk::DescriptorSetLayout> Pass::_getDescriptorSetLayout()
	{
		return m_pDescriptorSetLayout;
	}

	std::shared_ptr<vk::DescriptorPool> Pass::_getDescriptorPool()
	{
		return m_pDescriptorPool;
	}

	std::shared_ptr<vk::DescriptorSet> Pass::_getDescriptorSet()
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
				if (item.isTexture == KGS_FALSE)
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
					totalSize += item.size;
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
			kgs::findMemoryType(pApp->getPhysicalDevice(),
			memReqs.memoryTypeBits,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
		};

		pBufferMemory = fd::allocateMemory(pDevice, allocateInfo);

		pDevice->bindBufferMemory(*pBuffer, *pBufferMemory, 0);
	}
}