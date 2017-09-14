#include "graphics/material/pass.hpp"

namespace kgs
{
	Pass::Pass() :
		m_pContext(pContext)
	{
	}

	Pass::Pass(std::shared_ptr<MaterialData> pMaterialData) :
		m_pContext(pContext),
		m_pMaterialData(pMaterialData)
	{
		_createBindLayout();
		_createDescriptorSetLayout();
		_createUniformBuffer();
		_createDescriptorSet();
		_updateDescriptorBufferInfo();
	}

	Pass::~Pass()
	{
	}

	void Pass::_setMaterialData(std::shared_ptr<MaterialData> pMaterialData)
	{
		m_pMaterialData = pMaterialData;
		_createBindLayout();
		_createDescriptorSetLayout();
		_createUniformBuffer();
		_createDescriptorSet();
		_updateDescriptorBufferInfo();
	}

	void Pass::apply()
	{
		_updateDescriptorImageInfo();
		_applyBufferContent();
	}

	void Pass::_createDescriptorSetLayout()
	{
		std::vector<vk::DescriptorSetLayoutBinding> bindings(m_binds.size());
		size_t index = 0;
		for (const auto& item : m_binds)
		{
			bindings[index].binding = item.binding;
			bindings[index].descriptorCount = item.descriptorCount;
			bindings[index].descriptorType = tranDescriptorTypeToVK(item.descriptorType);
			bindings[index].stageFlags = tranShaderStageFlagsToVK(item.stageFlags);
			++index;

		}

		auto pDevice = m_pContext->getPNativeDevice();

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
		for (const auto& item : m_binds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
			{
				size = sizeof(MaterialData::getDataBaseType(item.dataType)) * item.descriptorCount;
				totalSize += size;
			}
		}
		createBuffer(static_cast<vk::DeviceSize>(totalSize), m_pUniformBuffer, m_pUniformBufferMemory);
	}

	void Pass::_createDescriptorSet()
	{
		std::unordered_map<vk::DescriptorType, uint32_t> mapTypeCounts;
		size_t index = 0;
		for (const auto& item : m_binds)
		{
			vk::DescriptorType vkDescriptorType = tranDescriptorTypeToVK(item.descriptorType);
			mapTypeCounts[vkDescriptorType] += item.descriptorCount; //??? + 1.
		}

		auto pDevice = m_pContext->getPNativeDevice();
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
		for (const auto& item : m_binds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
				++count;
		}

		std::vector<vk::WriteDescriptorSet> writes(count);

		uint32_t offset = 0u;
		uint32_t index = 0u;
		uint32_t size = 0u;
		for (const auto& item : m_binds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
			{
				std::vector<vk::DescriptorBufferInfo> bufferInfos(item.descriptorCount);
				for (uint32_t i = 0; i < item.descriptorCount; ++i)
				{
					size = sizeof(MaterialData::getDataBaseType(item.dataType));
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

		auto pDevice = m_pContext->getPNativeDevice();
		pDevice->updateDescriptorSets(writes, nullptr);
	}

	void Pass::_updateDescriptorImageInfo()
	{
		//get total number of unimform buffer variables.
		int32_t count = 0u;
		for (const auto& item : m_binds)
		{
			if (item.descriptorType == DescriptorType::COMBINED_IMAGE_SAMPLER)
				++count;
		}

		std::vector<vk::WriteDescriptorSet> writes(count);

		uint32_t index = 0u;
		for (const auto& item : m_binds)
		{
			if (item.descriptorType == DescriptorType::COMBINED_IMAGE_SAMPLER)
			{
#ifdef DEBUG
				if (item.dataType != MaterialData::DataType::TEXTURE)
					throw std::runtime_error("The data type of binding should be is TEXTURE when its type is COMBINED_IMAGE_SAMPLER");
				if (item.descriptorCount != 1)
					throw std::runtime_error("The descriptor count of binding shoubld be 1 when its type is COMBINED_IMAGE_SAMPLER");
#endif // DEBUG
				std::shared_ptr<Texture> pTexture = m_pMaterialData->getDataValue<MaterialData::DataType::TEXTURE>(item.name);
				vk::DescriptorImageInfo imageInfo;
				if (pTexture != nullptr)
				{
					imageInfo.sampler = *pTexture->_getPSampler();
					imageInfo.imageView = *pTexture->_getPImageView();
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

		auto pDevice = m_pContext->getPNativeDevice();
		pDevice->updateDescriptorSets(writes, nullptr);
	}

	void Pass::_applyBufferContent()
	{
		//get total number of unimform buffer variables.
		int32_t uniformBufferCount = 0u;
		for (const auto& item : m_binds)
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
		for (const auto& item : m_binds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
			{
				offsets[index] = offset;
				types[index] = item.dataType;
				names[index] = item.name;
				descriptorCounts[index] = item.descriptorCount;
				size = sizeof(MaterialData::getDataBaseType(item.dataType)) * item.descriptorCount;
				totalSize += size;
				offset += size;
				++index;
			}
		}
		//sync buffer data.
		void *data;
		auto pDevice = m_pContext->getPNativeDevice();
		pDevice->mapMemory(*m_pUniformBufferMemory, 0, static_cast<vk::DeviceSize>(totalSize), vk::MemoryMapFlags(), &data);
		for (int32_t i = 0; i < uniformBufferCount; ++i)
		{
			m_pMaterialData->memCopyDataValue(names[i], types[i], data, offsets[i], descriptorCounts[i]);
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

		auto pDevice = m_pContext->getPNativeDevice();
		pBuffer = fd::createBuffer(pDevice, createInfo);

		vk::MemoryRequirements memReqs = pDevice->getBufferMemoryRequirements(*pBuffer);
		vk::MemoryAllocateInfo allocateInfo = {
			memReqs.size,
			kgs::_findMemoryType(m_pContext->getPPhysicalDevice(),
			memReqs.memoryTypeBits,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
		};

		pBufferMemory = fd::allocateMemory(pDevice, allocateInfo);

		pDevice->bindBufferMemory(*pBuffer, *pBufferMemory, 0);
	}
}