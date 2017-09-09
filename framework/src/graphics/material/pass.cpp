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
		_updateDescriptorSetAndLayout();
	}

	Pass::~Pass()
	{
	}

	void Pass::_setMaterialData(std::shared_ptr<MaterialData> pMaterialData)
	{
		m_pMaterialData = pMaterialData;
		_createBindLayout();
		_updateDescriptorSetAndLayout();
	}

	void Pass::_updateDescriptorSetAndLayout()
	{
		std::vector<vk::DescriptorSetLayoutBinding> bindings(m_binds.size());
		std::unordered_map<vk::DescriptorType, uint32_t> mapTypeCounts;
		size_t index = 0;
		for (const auto& item : m_binds)
		{
			bindings[index].binding = item.binding;
			bindings[index].descriptorCount = 1;

			vk::DescriptorType vkDescriptorType;
#ifdef DEBUG
			Bool32 isHave = KGS_FALSE;
#endif // DEBUG
			for (const auto& item2 : arrDescriptorTypeToVK)
			{
				if (item2.first == item.descriptorType)
				{
					vkDescriptorType = item2.second;
#ifdef DEBUG
					isHave = KGS_TRUE;
#endif // DEBUG
					break;
				}
			}

#ifdef DEBUG
			if (isHave == KGS_FALSE)
			{
				throw std::runtime_error("Invalid descriptor type ");
			}
#endif // DEBUG

			bindings[index].descriptorType = vkDescriptorType;
			bindings[index].stageFlags = tranShaderStageFlagsToVK(item.stageFlags);
			++index;

			++mapTypeCounts[vkDescriptorType];
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

	void Pass::_updateDescriptorSetContent()
	{
		//get total number of unimform buffer variables.
		int32_t uniformBufferCount;
		for (const auto& item : m_binds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
				++uniformBufferCount;
		}

		//get total size of uniform buffer data and their offsets and sizes for each.
		size_t totalSize;
		std::vector<uint32_t> offsets(uniformBufferCount);
		std::vector<uint32_t> sizes(uniformBufferCount);
		for (const auto& item : m_binds)
		{
			if (item.descriptorType == DescriptorType::UNIFORM_BUFFER)
			{
				//auto map = static_cast<std::unordered_map<std::string, void*>>(*(m_pMaterialData->arrTypeToMap[static_cast<uint32_t>(item.dataType)]));
			}
		}
		
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