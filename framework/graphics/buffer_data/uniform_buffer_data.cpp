#include "graphics/buffer_data/uniform_buffer_data.hpp"

#include "graphics/module.hpp"
#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/buffer_data/util.hpp"

namespace vg
{
    UniformBufferData::DescriptorBufferInfo::DescriptorBufferInfo()
        : range(0u)
        , bufferRange(0u)
    {

    }

    UniformBufferData::DescriptorBufferInfo::DescriptorBufferInfo(uint32_t range, uint32_t bufferRange)
        : range(range)
        , bufferRange(bufferRange)
    {

    }

    Bool32  UniformBufferData::DescriptorBufferInfo::operator ==(const DescriptorBufferInfo& target) const
    {
        if (range == target.range && bufferRange == target.bufferRange) return VG_TRUE;
        return VG_FALSE;
    }

	Bool32  UniformBufferData::DescriptorBufferInfo::operator !=(const DescriptorBufferInfo& target) const
    {
        if ((*this) == target) return VG_FALSE;
        return VG_TRUE;
    }

    UniformBufferData::DescriptorImageInfo::DescriptorImageInfo()
        : sampler(nullptr)
        , imageView(nullptr)
        , imageLayout(vk::ImageLayout::eUndefined)
    {

    }

    UniformBufferData::DescriptorImageInfo::DescriptorImageInfo(vk::Sampler sampler
        , vk::ImageView imageView
        , vk::ImageLayout imageLayout
        )
        : sampler(sampler)
        , imageView(imageView)
        , imageLayout(imageLayout)

    {

    }

    UniformBufferData::SubDataInfo::SubDataInfo()
        : layoutBindingCount(0u)
        , pLayoutBindings(nullptr)
        , pDescriptorInfos(nullptr)
        , bufferOffset(0u)
    {

    }

    UniformBufferData::SubDataInfo::SubDataInfo(uint32_t layoutBindingCount
        , vk::DescriptorSetLayoutBinding *pLayoutBindings
        , DescriptorBufferInfo *pDescriptorInfos
        , uint32_t bufferOffset
        )
        : layoutBindingCount(layoutBindingCount)
        , pLayoutBindings(pLayoutBindings)
        , pDescriptorInfos(pDescriptorInfos)
        , bufferOffset(bufferOffset)
    {

    }

    UniformBufferData::SubData::SubData()
        : m_layoutBindingCount(0u)
        , m_layoutBindings()
        , m_descriptorInfos()
        , m_bufferOffset()
        , m_pDescriptorSetLayout(nullptr)
        , m_pDescriptorSet(nullptr)
        , m_descriptorPool(nullptr)
        , m_buffer(nullptr)
    {

    }

    UniformBufferData::SubData::SubData(SubDataInfo info, vk::Buffer *pBuffer, vk::DescriptorPool *pDescriptorPool)
        : m_layoutBindingCount(0u)
        , m_layoutBindings()
        , m_descriptorInfos()
        , m_bufferOffset()
        , m_pDescriptorSetLayout(nullptr)
        , m_pDescriptorSet(nullptr)
        , m_descriptorPool(nullptr)
        , m_buffer(nullptr)
    {
        init(info, pBuffer, pDescriptorPool);
    }

    void UniformBufferData::SubData::init(SubDataInfo info, vk::Buffer *pBuffer, vk::DescriptorPool *pDescriptorPool)
    {
        //Check if layout binding is changed.
        Bool32 layoutBindingChanged = VG_FALSE;
        if (m_layoutBindingCount != info.layoutBindingCount) {
            layoutBindingChanged = VG_TRUE;
        } else {
            for (uint32_t i = 0; i < info.layoutBindingCount; ++i) {
                auto item1 = m_layoutBindings[i];
                auto item2 = *(info.pLayoutBindings + i);
                if (item1 != item2) {
                    layoutBindingChanged = VG_TRUE;
                    break;
                }
            }
        }

        if (layoutBindingChanged == VG_TRUE)
        {
            //Copy layout bindings data.
            m_layoutBindingCount = info.layoutBindingCount;
            m_layoutBindings.resize(info.layoutBindingCount);
            memcpy(m_layoutBindings.data(), info.pLayoutBindings, sizeof(vk::DescriptorSetLayoutBinding) * static_cast<size_t>(info.layoutBindingCount));

            //Create descriptor set layout.
            auto pDevice = pApp->getDevice();
		    if (m_layoutBindingCount)
		    {
		    	vk::DescriptorSetLayoutCreateInfo createInfo =
		    	{
		    		vk::DescriptorSetLayoutCreateFlags(),
		    		static_cast<uint32_t>(m_layoutBindingCount),
		    		m_layoutBindings.data()
		    	};
		    	m_pDescriptorSetLayout = fd::createDescriptorSetLayout(pDevice, createInfo);
		    }
            else
            {
                m_pDescriptorSetLayout = nullptr;
            }

        }

        vk::DescriptorPool descriptorPool = pDescriptorPool != nullptr ? *(pDescriptorPool) : vk::DescriptorPool(nullptr);

        Bool32 poolChanged = m_descriptorPool != descriptorPool ? VG_TRUE : VG_FALSE;
        if (layoutBindingChanged || poolChanged)
        {
            //Create descriptor set.
            auto pDevice = pApp->getDevice();
		    if (m_pDescriptorSetLayout != nullptr && descriptorPool != vk::DescriptorPool(nullptr))
		    {
		    	vk::DescriptorSetLayout layouts[] = { *m_pDescriptorSetLayout };
		    	vk::DescriptorSetAllocateInfo allocateInfo = {
		    		descriptorPool,
		    		1u,
		    		layouts
		    	};
		    	m_pDescriptorSet = fd::allocateDescriptorSet(pDevice, &descriptorPool, allocateInfo);
		    }
		    else
		    {
		    	m_pDescriptorSet = nullptr;
		    }
        }

        //Copy descriptor pool.
        if (poolChanged) m_descriptorPool = descriptorPool;

        //Check if descriptor infos  is changed.

        uint32_t descriptorInfoCount = 0u;
        for (uint32_t i = 0; i < info.layoutBindingCount; ++i)
        {
            descriptorInfoCount += m_layoutBindings[i].descriptorCount;
        }

        Bool32 descriptorInfosChanged = VG_FALSE;
        if (static_cast<uint32_t>(m_descriptorInfos.size()) != descriptorInfoCount) {
            descriptorInfosChanged = VG_TRUE;
        } else {
            for (uint32_t i = 0; i < descriptorInfoCount; ++i) {
                auto item1 = m_descriptorInfos[i];
                auto item2 = *(info.pDescriptorInfos + i);
                if (item1 != item2) {
                    descriptorInfosChanged = VG_TRUE;
                    break;
                }
            }
        }

        vk::Buffer buffer = pBuffer != nullptr ? *pBuffer : vk::Buffer(nullptr);
        
        Bool32 bufferChanged = m_buffer != buffer ? VG_TRUE : VG_FALSE;

        if (descriptorInfosChanged)
        {
            //Copy descriptor infos data
            m_descriptorInfos.resize(descriptorInfoCount);
            memcpy(m_descriptorInfos.data(), info.pDescriptorInfos, sizeof(DescriptorBufferInfo) * static_cast<size_t>(descriptorInfoCount));
        }

        m_bufferOffset = info.bufferOffset;

        if (descriptorInfoCount && buffer != vk::Buffer() && 
            (descriptorInfosChanged || bufferChanged || layoutBindingChanged || poolChanged))
        {
            std::vector<vk::WriteDescriptorSet> writes(m_layoutBindingCount);
		    std::vector<std::vector<vk::DescriptorBufferInfo>> bufferInfoss(m_layoutBindingCount);
		    uint32_t offset = info.bufferOffset;
		    uint32_t descriptorInfoIndex = 0u;
            for (uint32_t bindingIndex = 0u; bindingIndex < m_layoutBindingCount; ++bindingIndex)
            {
                const auto &layoutBinding = m_layoutBindings[bindingIndex];
                bufferInfoss[bindingIndex].resize(layoutBinding.descriptorCount);
                for (uint32_t descriptorIndex = 0u; descriptorIndex < layoutBinding.descriptorCount; ++descriptorIndex)
                {
                    auto &descriptorInfo = m_descriptorInfos[descriptorInfoIndex];
                    bufferInfoss[bindingIndex][descriptorIndex].buffer = buffer;
                    bufferInfoss[bindingIndex][descriptorIndex].offset = offset;
                    bufferInfoss[bindingIndex][descriptorIndex].range = descriptorInfo.range;
                    offset += descriptorInfo.bufferRange;
                    ++descriptorInfoIndex;
                }
                writes[bindingIndex].dstSet = *m_pDescriptorSet;
		    	writes[bindingIndex].dstBinding = layoutBinding.binding;
		    	writes[bindingIndex].descriptorType = layoutBinding.descriptorType;
		    	writes[bindingIndex].dstArrayElement = 0;
		    	writes[bindingIndex].descriptorCount = layoutBinding.descriptorCount;
		    	writes[bindingIndex].pBufferInfo = bufferInfoss[bindingIndex].data();
            }
		    auto pDevice = pApp->getDevice();
		    pDevice->updateDescriptorSets(writes, nullptr);
        }

        if (bufferChanged) m_buffer = buffer;
    }

    uint32_t UniformBufferData::SubData::getLayoutBindingCount() const
    {
        return m_layoutBindingCount;
    }

    const vk::DescriptorSetLayoutBinding *UniformBufferData::SubData::getLayoutBindings() const
    {
        return m_layoutBindings.data();
    }
    
    uint32_t UniformBufferData::SubData::getDescriptorInfoCount() const
    {
        return m_descriptorInfos.size();
    }
    
    const UniformBufferData::DescriptorBufferInfo *UniformBufferData::SubData::getDescriptorBufferInfos() const
    {
        return m_descriptorInfos.data();
    }
            
    uint32_t UniformBufferData::SubData::getBufferOffset() const
    {
        return m_bufferOffset;
    }
            
    const vk::DescriptorSetLayout *UniformBufferData::SubData::getDescriptorSetLayout() const
    {
        return m_pDescriptorSetLayout.get();
    }

    const vk::DescriptorSet *UniformBufferData::SubData::getDescriptorSet() const
    {
        return m_pDescriptorSet.get();
    }

    UniformBufferData::UniformBufferData()
        : Base(BaseType::UNIFORM_BUFFER_DATA)
        , m_bufferMemoryPropertyFlags()
        , m_subDataCount()
        , m_subDatas()
        , m_bufferSize()
        , m_pBuffer()
        , m_bufferMemorySize()
        , m_pBufferMemory()
        , m_memorySize(0u)
        , m_pMemory(nullptr)
        , m_pMmemoryForHostVisible(nullptr)
        , m_poolSizeInfos()
        , m_pDescriptorPool()
    {
        //default is device local.
        if (! m_bufferMemoryPropertyFlags) 
        {
            m_bufferMemoryPropertyFlags |= MemoryPropertyFlagBits::DEVICE_LOCAL;
        }
    }
        
    UniformBufferData::UniformBufferData(MemoryPropertyFlags bufferMemoryPropertyFlags)
        : Base(BaseType::UNIFORM_BUFFER_DATA)
        , m_bufferMemoryPropertyFlags(bufferMemoryPropertyFlags)
        , m_subDataCount()
        , m_subDatas()
        , m_bufferSize()
        , m_pBuffer()
        , m_bufferMemorySize()
        , m_pBufferMemory()
        , m_memorySize(0u)
        , m_pMemory(nullptr)
        , m_pMmemoryForHostVisible(nullptr)
        , m_poolSizeInfos()
        , m_pDescriptorPool()
    {
        //default is device local.
        if (! m_bufferMemoryPropertyFlags) 
        {
            m_bufferMemoryPropertyFlags |= MemoryPropertyFlagBits::DEVICE_LOCAL;
        }
    }

    UniformBufferData::~UniformBufferData()
    {
        if (m_pMemory != nullptr)
        {
            free(m_pMemory);
        }

    }

    void UniformBufferData::init(uint32_t subDataCount
            , const SubDataInfo *pSubDataInfos
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            )
    {
        updateBuffer(memory, size, cacheMemory);
        updateDesData(subDataCount, pSubDataInfos);
    }

    void UniformBufferData::updateBuffer(const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            )
    {
        MemorySlice slice;
        slice.offset = 0u;
        slice.size = size;
        slice.pMemory = memory;
        updateBuffer(slice, size, cacheMemory);
    }

    void UniformBufferData::updateBuffer(fd::ArrayProxy<MemorySlice> memories
        , uint32_t size
        , Bool32 cacheMemory
        )
    {
        //Caching memory when memory is device local.
        cacheMemory = cacheMemory && _isDeviceMemoryLocal();
        if (m_pMemory != nullptr && (m_memorySize < size || ! cacheMemory)) {
            free(m_pMemory);
            m_pMemory = nullptr;
            m_memorySize = 0;
        }

        if (size)
		{
			if (cacheMemory) {
				if (m_pMemory == nullptr) {
					m_pMemory = malloc(size);
					m_memorySize = size;
				}
				uint32_t count = memories.size();
				uint32_t offset = 0;
				uint32_t size = 0;
				for (uint32_t i = 0; i < count; ++i) {
					offset = (*(memories.data() + i)).offset;
					size = (*(memories.data() + i)).size;
					memcpy(((char*)m_pMemory + offset), (*(memories.data() + i)).pMemory, size);
				}
			}
			_createBuffer(memories, size);
		}
    }

    void UniformBufferData::updateSubDataCount(uint32_t count)
    {
        m_subDatas.resize(count);
        m_subDataCount = count;
    }

    void UniformBufferData::updateDesData(uint32_t subDataCount, const SubDataInfo *pSubDataInfos, uint32_t subDataOffset)
    {
        if (_isEqual(m_subDataCount, m_subDatas.data(), subDataOffset, 
            subDataCount, pSubDataInfos, subDataOffset) == VG_FALSE)
        {
            //Check and reCreate descriptor pool.
            //Caculate current need pool size info.
            std::unordered_map<vk::DescriptorType, uint32_t> poolSizeInfos;
            for (uint32_t i = 0; i < subDataOffset && i < m_subDataCount; ++i) 
            {
                auto &subData = m_subDatas[i];
                uint32_t bindingCount = subData.getLayoutBindingCount();
                for (uint32_t j = 0; j < bindingCount; ++j)
                {
                    auto &binding = *(subData.getLayoutBindings() + j);
                    poolSizeInfos[binding.descriptorType] += binding.descriptorCount;
                }
            }

            for (uint32_t i = 0; i < subDataCount; ++i)
            {
                auto &subDataInfo = pSubDataInfos[i];
                uint32_t bindingCount = subDataInfo.layoutBindingCount;
                for (uint32_t j = 0; j < bindingCount; ++j)
                {
                    auto &binding = *(subDataInfo.pLayoutBindings + j);
                    poolSizeInfos[binding.descriptorType] += binding.descriptorCount;
                }
            }

            for (uint32_t i = subDataOffset + subDataCount; i < m_subDataCount; ++i)
            {
                auto &subData = m_subDatas[i];
                uint32_t bindingCount = subData.getLayoutBindingCount();
                for (uint32_t j = 0; j < bindingCount; ++j)
                {
                    auto &binding = *(subData.getLayoutBindings() + j);
                    poolSizeInfos[binding.descriptorType] += binding.descriptorCount;
                }
            }

            //Check pool size is greater than need. if not, recreating pool.
            Bool32 isGreater = VG_TRUE;
		    for (const auto &pair : poolSizeInfos) {
                if (m_poolSizeInfos[pair.first] < poolSizeInfos[pair.first]) {
                    isGreater = VG_FALSE;
                    break;
                }
		    }

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

            }


            if (m_subDataCount < subDataOffset + subDataCount) 
            {
                m_subDataCount = subDataOffset + subDataCount;
                m_subDatas.resize(m_subDataCount);
            }

            uint32_t offset = subDataOffset;
            for (uint32_t i = 0; i < subDataCount; ++i, ++offset)
            {
                auto &subData = m_subDatas[offset];
                subData.init(*(pSubDataInfos + i), m_pBuffer.get(), m_pDescriptorPool.get());
            }
        }
    }

    uint32_t UniformBufferData::getSubDataCount() const
    {
        return m_subDataCount;
    }

    const UniformBufferData::SubData *UniformBufferData::getSubDatas() const
    {
        return m_subDatas.data();
    }
        
    uint32_t UniformBufferData::getBufferSize() const
    {
        return m_bufferSize;
    }

    const vk::Buffer *UniformBufferData::getBuffer() const
    {
        return m_pBuffer.get();
    }

    uint32_t UniformBufferData::getBufferMemorySize() const
    {
        return m_bufferMemorySize;
    }

    const vk::DeviceMemory *UniformBufferData::getBufferMemory() const
    {
        return m_pBufferMemory.get();
    }

    uint32_t UniformBufferData::getMemorySize() const
    {
        return m_memorySize;
    }

    const void *UniformBufferData::getMemory() const
    {
        return m_pMemory;
    }

    Bool32 UniformBufferData::_isDeviceMemoryLocal() const
    {
        return (m_bufferMemoryPropertyFlags & MemoryPropertyFlagBits::DEVICE_LOCAL) == MemoryPropertyFlagBits::DEVICE_LOCAL;
    }

    void UniformBufferData::_createBuffer(fd::ArrayProxy<MemorySlice> memories, uint32_t memorySize)
    {
		createBufferForBufferData(memories, 
            memorySize, 
            _isDeviceMemoryLocal(), 
            vk::BufferUsageFlagBits::eVertexBuffer,
            m_bufferSize,
            m_pBuffer,
            m_bufferMemorySize,
            m_pBufferMemory,
            &m_pMmemoryForHostVisible);
    }

} //vg