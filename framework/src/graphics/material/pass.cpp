#include "graphics/material/pass.hpp"

namespace kgs
{
	Pass::Pass() :
		m_pContext(pContext),
		m_pData(new MaterialData())
	{
	}

	Pass::~Pass()
	{
	}

	std::shared_ptr<Texture> Pass::getMainTexture()
	{
		return getData<MaterialData::DataType::TEXTURE>(KGS_M_MAIN_TEXTURE_NAME);
	}

	void Pass::setMainTexture(std::shared_ptr<Texture> value)
	{
		setData<MaterialData::DataType::TEXTURE>(KGS_M_MAIN_TEXTURE_NAME, value, KGS_M_MAIN_TEXTURE_BINDING, 
			DescriptorType::COMBINED_IMAGE_SAMPLER, ShaderStageFlagBits::FRAGMENT);
	}

	Vector2 Pass::getMainTextureOffset()
	{
		return getData<MaterialData::DataType::TEXTURE_OFFSET>(KGS_M_MAIN_TEXTURE_NAME);
	}

	void Pass::setMainTextureOffset(Vector2 value)
	{
		setData<MaterialData::DataType::TEXTURE_OFFSET>(KGS_M_MAIN_TEXTURE_NAME, value, KGS_M_MAIN_TEXTURE_OFFSET_BINDING, 
			DescriptorType::UNIFORM_BUFFER, ShaderStageFlagBits::FRAGMENT);
	}

	Vector2 Pass::getMainTextureScale()
	{
		return getData<MaterialData::DataType::TEXTURE_SCALE>(KGS_M_MAIN_TEXTURE_NAME);
	}

	void Pass::setMainTextureScale(Vector2 value)
	{
		setData<MaterialData::DataType::TEXTURE_SCALE>(KGS_M_MAIN_TEXTURE_NAME, value, KGS_M_MAIN_TEXTURE_SCALE_BINDING,
			DescriptorType::UNIFORM_BUFFER, ShaderStageFlagBits::FRAGMENT);
	}

	Color Pass::getMainColor()
	{
		return getData<MaterialData::DataType::COLOR>(KGS_M_MAIN_COLOR_NAME);
	}

	void Pass::setMainColor(Color value)
	{
		setData<MaterialData::DataType::COLOR>(KGS_M_MAIN_COLOR_NAME, value,
			KGS_M_MAIN_COLOR_BINDING, DescriptorType::UNIFORM_BUFFER, ShaderStageFlagBits::VERTEX);
	}

	void Pass::apply()
	{
		_createDescriptorSetLayout();
		_createUniformBuffer();
		_createDescriptorSet();
		_updateDescriptorBufferInfo();
		_updateDescriptorImageInfo();
		_applyBufferContent();
	}

	void Pass::_createDescriptorSetLayout()
	{
		std::vector<vk::DescriptorSetLayoutBinding> bindings(m_arrLayoutBinds.size());
		size_t index = 0;
		for (const auto& item : m_arrLayoutBinds)
		{
			bindings[index].binding = item.binding;
			bindings[index].descriptorCount = item.descriptorCount;
			bindings[index].descriptorType = tranDescriptorTypeToVK(item.descriptorType);
			bindings[index].stageFlags = tranShaderStageFlagsToVK(item.stageFlags);
			++index;

		}

		auto pDevice = m_pContext->getNativeDevice();

		//create descriptor set layout.
		{
			vk::DescriptorSetLayoutCreateInfo createInfo =
			{
				vk::DescriptorSetLayoutCreateFlags(),
				static_cast<uint32_t>(bindings.size()),
				bindings.data()
			};

			m_pDescriptorSetLayout = fd::createDescriptorSetLayout(pDevice, createInfo);
		}
	}

	void Pass::_createUniformBuffer()
	{
		//get total size of uniform buffer datas and their offsets and sizes for each one.
		uint32_t totalSize = 0u;
		uint32_t size = 0u;
		for (const auto& item : m_arrLayoutBinds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
			{
				size = sizeof(MaterialData::getDataBaseTypeSize(item.dataType)) * item.descriptorCount;
				totalSize += size;
			}
		}
		createBuffer(static_cast<vk::DeviceSize>(totalSize), m_pUniformBuffer, m_pUniformBufferMemory);
	}

	void Pass::_createDescriptorSet()
	{
		std::unordered_map<vk::DescriptorType, uint32_t> mapTypeCounts;
		size_t index = 0;
		for (const auto& item : m_arrLayoutBinds)
		{
			vk::DescriptorType vkDescriptorType = tranDescriptorTypeToVK(item.descriptorType);
			mapTypeCounts[vkDescriptorType] += item.descriptorCount; //??? + 1.
		}

		auto pDevice = m_pContext->getNativeDevice();
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

			vk::DescriptorPoolCreateInfo createInfo = { vk::DescriptorPoolCreateFlags(), 1u, static_cast<uint32_t>(poolSizeInfos.size()), poolSizeInfos.data() };
			m_pDescriptorPool = fd::createDescriptorPool(pDevice, createInfo);
		}

		//create descriptor set.
		vk::DescriptorSetLayout layouts[] = { *m_pDescriptorSetLayout };
		vk::DescriptorSetAllocateInfo allocateInfo = {
			*m_pDescriptorPool,
			1u,
			layouts
		};
		m_pDescriptorSet = fd::allocateDescriptorSet(pDevice, m_pDescriptorPool, allocateInfo);
	}

	void Pass::_updateDescriptorBufferInfo()
	{
		//get total number of unimform buffer variables.
		int32_t count = 0u;
		for (const auto& item : m_arrLayoutBinds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
				++count;
		}

		std::vector<vk::WriteDescriptorSet> writes(count);

		uint32_t offset = 0u;
		uint32_t index = 0u;
		uint32_t size = 0u;
		for (const auto& item : m_arrLayoutBinds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
			{
				std::vector<vk::DescriptorBufferInfo> bufferInfos(item.descriptorCount);
				for (uint32_t i = 0; i < item.descriptorCount; ++i)
				{
					size = sizeof(MaterialData::getDataBaseTypeSize(item.dataType));
					bufferInfos[i].buffer = *m_pUniformBuffer;
					bufferInfos[i].offset = offset;
					bufferInfos[i].range = size;
					offset += size;
				}
				writes[index].dstSet = *m_pDescriptorSet;
				writes[index].dstBinding = item.binding;
				writes[index].descriptorType = tranDescriptorTypeToVK(item.descriptorType);
				writes[index].dstArrayElement = 0;
				writes[index].descriptorCount = item.descriptorCount;
				writes[index].pBufferInfo = bufferInfos.data();
				++index;
			}
		}

		auto pDevice = m_pContext->getNativeDevice();
		pDevice->updateDescriptorSets(writes, nullptr);
	}

	void Pass::_updateDescriptorImageInfo()
	{
		//get total number of unimform buffer variables.
		int32_t count = 0u;
		for (const auto& item : m_arrLayoutBinds)
		{
			if (item.descriptorType == DescriptorType::COMBINED_IMAGE_SAMPLER)
				++count;
		}

		std::vector<vk::WriteDescriptorSet> writes(count);

		uint32_t index = 0u;
		for (const auto& item : m_arrLayoutBinds)
		{
			if (item.descriptorType == DescriptorType::COMBINED_IMAGE_SAMPLER)
			{
#ifdef DEBUG
				if (item.dataType != MaterialData::DataType::TEXTURE)
					throw std::runtime_error("The data type of binding should be is TEXTURE when its type is COMBINED_IMAGE_SAMPLER");
				if (item.descriptorCount != 1)
					throw std::runtime_error("The descriptor count of binding shoubld be 1 when its type is COMBINED_IMAGE_SAMPLER");
#endif // DEBUG
				std::shared_ptr<Texture> pTexture = m_pData->getDataValue<MaterialData::DataType::TEXTURE>(item.name);
				vk::DescriptorImageInfo imageInfo;
				if (pTexture != nullptr)
				{
					imageInfo.sampler = *pTexture->_getSampler();
					imageInfo.imageView = *pTexture->_getImageView();
					imageInfo.imageLayout = pTexture->_getImageLayout();
				}

				writes[index].dstSet = *m_pDescriptorSet;
				writes[index].dstBinding = item.binding;
				writes[index].descriptorType = tranDescriptorTypeToVK(item.descriptorType);
				writes[index].dstArrayElement = 0u;
				writes[index].descriptorCount = item.descriptorCount;
				writes[index].pImageInfo = &imageInfo;
				++index;
			}
		}

		auto pDevice = m_pContext->getNativeDevice();
		pDevice->updateDescriptorSets(writes, nullptr);
	}

	void Pass::_applyBufferContent()
	{
		//get total number of unimform buffer variables.
		int32_t uniformBufferCount = 0u;
		for (const auto& item : m_arrLayoutBinds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
				++uniformBufferCount;
		}

		//get total size of uniform buffer datas and their offsets and sizes for each one.
		uint32_t totalSize = 0u;
		uint32_t offset = 0u;
		std::vector<uint32_t> offsets(uniformBufferCount);
		std::vector<std::string> names(uniformBufferCount);
		std::vector<MaterialData::DataType> types(uniformBufferCount);
		std::vector<uint32_t> descriptorCounts(uniformBufferCount);
		uint32_t index = 0u;
		uint32_t size = 0u;
		for (const auto& item : m_arrLayoutBinds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
			{
				offsets[index] = offset;
				types[index] = item.dataType;
				names[index] = item.name;
				descriptorCounts[index] = item.descriptorCount;
				size = sizeof(MaterialData::getDataBaseTypeSize(item.dataType)) * item.descriptorCount;
				totalSize += size;
				offset += size;
				++index;
			}
		}
		//sync buffer data.
		void *data;
		auto pDevice = m_pContext->getNativeDevice();
		pDevice->mapMemory(*m_pUniformBufferMemory, 0, static_cast<vk::DeviceSize>(totalSize), vk::MemoryMapFlags(), &data);
		for (int32_t i = 0; i < uniformBufferCount; ++i)
		{
			m_pData->memCopyDataValue(names[i], types[i], data, offsets[i], 0u, descriptorCounts[i]);
		}
		pDevice->unmapMemory(*m_pUniformBufferMemory);
	}

	void Pass::createBuffer(vk::DeviceSize size, std::shared_ptr<vk::Buffer>& pBuffer, std::shared_ptr<vk::DeviceMemory> pBufferMemory)
	{
		vk::BufferCreateInfo createInfo = {
			vk::BufferCreateFlags(),
			size,
			vk::BufferUsageFlagBits::eUniformBuffer,
			vk::SharingMode::eExclusive
		};

		auto pDevice = m_pContext->getNativeDevice();
		pBuffer = fd::createBuffer(pDevice, createInfo);

		vk::MemoryRequirements memReqs = pDevice->getBufferMemoryRequirements(*pBuffer);
		vk::MemoryAllocateInfo allocateInfo = {
			memReqs.size,
			kgs::findMemoryType(m_pContext->getPhysicalDevice(),
			memReqs.memoryTypeBits,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
		};

		pBufferMemory = fd::allocateMemory(pDevice, allocateInfo);

		pDevice->bindBufferMemory(*pBuffer, *pBufferMemory, 0);
	}
}