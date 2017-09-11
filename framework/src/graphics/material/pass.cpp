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
		uint32_t totalSize;
		uint32_t size;
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

			vk::DescriptorPoolCreateInfo createInfo = { vk::DescriptorPoolCreateFlags(), 1u, poolSizeInfos.size(), poolSizeInfos.data() };
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
		int32_t count;
		for (const auto& item : m_binds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
				++count;
		}

		std::vector<vk::WriteDescriptorSet> writes(count);

		uint32_t offset;
		uint32_t index;
		uint32_t size;
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

		/*VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;*/

		/*descriptorSetWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorSetWrites[1].dstSet = descriptorSet;
		descriptorSetWrites[1].dstBinding = 1;
		descriptorSetWrites[1].dstArrayElement = 0;
		descriptorSetWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorSetWrites[1].descriptorCount = 1;
		descriptorSetWrites[1].pImageInfo = &imageInfo;*/

		auto pDevice = m_pContext->getPNativeDevice();
		pDevice->updateDescriptorSets(writes, nullptr);
	}

	void Pass::_applyBufferContent()
	{
		//get total number of unimform buffer variables.
		int32_t uniformBufferCount;
		for (const auto& item : m_binds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
				++uniformBufferCount;
		}

		//get total size of uniform buffer datas and their offsets and sizes for each one.
		uint32_t totalSize;
		uint32_t offset;
		std::vector<uint32_t> offsets(uniformBufferCount);
		std::vector<std::string> names(uniformBufferCount);
		std::vector<MaterialData::DataType> types(uniformBufferCount);
		uint32_t index;
		uint32_t size;
		for (const auto& item : m_binds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
			{
				types[index] = item.dataType;
				names[index] = item.name;
				size = m_pMaterialData->getDataValueSize(item.name, item.dataType);
				offsets[index] = offset;
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
			m_pMaterialData->memCopyDataValue(names[i], types[i], data, offsets[i]);
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