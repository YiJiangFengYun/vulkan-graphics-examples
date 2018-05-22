#include "graphics/buffer_data/uniform_buffer_data.hpp"

#include "graphics/app/app.hpp"
#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/buffer_data/util.hpp"

namespace vg
{
    UniformBufferData::DescriptorBufferInfo::DescriptorBufferInfo(uint32_t range, uint32_t bufferRange, uint32_t dynamicOffset)
        : range(range)
        , bufferRange(bufferRange)
        , dynamicOffset(dynamicOffset)
    {

    }

    Bool32  UniformBufferData::DescriptorBufferInfo::operator ==(const DescriptorBufferInfo& target) const
    {
        if (range == target.range && bufferRange == target.bufferRange && dynamicOffset == target.dynamicOffset) return VG_TRUE;
        return VG_FALSE;
    }

	Bool32  UniformBufferData::DescriptorBufferInfo::operator !=(const DescriptorBufferInfo& target) const
    {
        if ((*this) == target) return VG_FALSE;
        return VG_TRUE;
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
        , m_pDescriptorPool(nullptr)
        , m_pBuffer(nullptr)
    {

    }

    UniformBufferData::SubData::SubData(SubDataInfo info
        , const vk::Buffer *pBuffer
        , const vk::DescriptorPool *pDescriptorPool)
        : m_layoutBindingCount(0u)
        , m_layoutBindings()
        , m_descriptorInfos()
        , m_bufferOffset()
        , m_pDescriptorSetLayout(nullptr)
        , m_pDescriptorSet(nullptr)
        , m_pDescriptorPool(nullptr)
        , m_pBuffer(nullptr)
    {
        init(info, pBuffer, pDescriptorPool);
    }

	UniformBufferData::SubData::~SubData()
	{
	}

    void UniformBufferData::SubData::init(SubDataInfo info
        , const vk::Buffer *pBuffer
        , const vk::DescriptorPool *pDescriptorPool)
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
		vk::DescriptorPool lastDescriptorPool = m_pDescriptorPool != nullptr ? *(m_pDescriptorPool) : vk::DescriptorPool(nullptr);
        Bool32 poolChanged = lastDescriptorPool != descriptorPool ? VG_TRUE : VG_FALSE;
		//Copy descriptor pool.
		if (poolChanged) m_pDescriptorPool = pDescriptorPool;
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
		    	m_pDescriptorSet = fd::allocateDescriptorSet(pDevice, m_pDescriptorPool, allocateInfo);
		    }
		    else
		    {
		    	m_pDescriptorSet = nullptr;
		    }
        }

        //Check if descriptor infos  is changed.
        uint32_t descriptorInfoCount = 0u;
        for (uint32_t i = 0; i < info.layoutBindingCount; ++i)
        {
            descriptorInfoCount += m_layoutBindings[i].descriptorCount;
        }

        Bool32 descriptorInfosChanged = VG_FALSE;
        Bool32 needWrite = VG_FALSE;
        if (static_cast<uint32_t>(m_descriptorInfos.size()) != descriptorInfoCount) {
            descriptorInfosChanged = VG_TRUE;
        } else {
            for (uint32_t i = 0; i < descriptorInfoCount; ++i) {
                auto item1 = m_descriptorInfos[i];
                auto item2 = *(info.pDescriptorInfos + i);
                if (item1 != item2) {
                    descriptorInfosChanged = VG_TRUE;
                }
                if (item1.range != item2.range || item1.bufferRange != item2.bufferRange)
                {
                    needWrite = VG_TRUE;
                }
                if (needWrite)
                {
                    break;
                }
            }
        }

        vk::Buffer buffer = pBuffer != nullptr ? *pBuffer : vk::Buffer(nullptr);
		vk::Buffer lastBuffer = m_pBuffer != nullptr ? *m_pBuffer : vk::Buffer(nullptr);
        Bool32 bufferChanged = lastBuffer != buffer ? VG_TRUE : VG_FALSE;
		if (bufferChanged) m_pBuffer = pBuffer;
        if (descriptorInfosChanged)
        {
            //Copy descriptor infos data
            m_descriptorInfos.resize(descriptorInfoCount);
            memcpy(m_descriptorInfos.data(), info.pDescriptorInfos, sizeof(DescriptorBufferInfo) * static_cast<size_t>(descriptorInfoCount));
        }

        m_bufferOffset = info.bufferOffset;

        if (descriptorInfoCount && buffer != vk::Buffer() && 
            (needWrite || bufferChanged || layoutBindingChanged || poolChanged))
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
    
    const UniformBufferData::DescriptorBufferInfo *UniformBufferData::SubData::getDescriptorInfos() const
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
        
    UniformBufferData::UniformBufferData(vk::MemoryPropertyFlags bufferMemoryPropertyFlags)
        : Base(BaseType::UNIFORM_BUFFER_DATA)
        , m_bufferData(bufferMemoryPropertyFlags ? bufferMemoryPropertyFlags : vk::MemoryPropertyFlagBits::eHostVisible)
        , m_subDataCount()
        , m_subDatas()
        , m_poolMaxSetCount()
        , m_poolSizeInfos(0u)
        , m_pDescriptorPool()
    {
    }

    UniformBufferData::~UniformBufferData()
    {
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
        m_bufferData.updateBuffer(memories, size, cacheMemory);
    }

    void UniformBufferData::updateSubDataCount(uint32_t count)
    {
        m_subDatas.resize(count);
        m_subDataCount = count;
    }

    void UniformBufferData::updateDesData(uint32_t subDataCount, const SubDataInfo *pSubDataInfos, uint32_t subDataOffset)
    {
        if (_isEqual(m_subDataCount, m_subDatas.data(), subDataOffset, 
            subDataCount, pSubDataInfos) == VG_FALSE)
        {
            //Check and reCreate descriptor pool.
            //Caculate current need pool size info.
            uint32_t poolMaxSetCount = 0u;
            std::unordered_map<vk::DescriptorType, uint32_t> poolSizeInfos;
            for (uint32_t i = 0; i < subDataOffset && i < m_subDataCount; ++i) 
            {
                auto &subData = m_subDatas[i];
                uint32_t bindingCount = subData.getLayoutBindingCount();
                poolMaxSetCount += bindingCount;                
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
                poolMaxSetCount += bindingCount; 
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
                poolMaxSetCount += bindingCount; 
                for (uint32_t j = 0; j < bindingCount; ++j)
                {
                    auto &binding = *(subData.getLayoutBindings() + j);
                    poolSizeInfos[binding.descriptorType] += binding.descriptorCount;
                }
            }

            //Check pool size is greater than need. if not, recreating pool.
            Bool32 isGreater = VG_TRUE;
            if (m_poolMaxSetCount < poolMaxSetCount)
            {
                isGreater = VG_FALSE;
            }
            else 
            {
                for (const auto &pair : poolSizeInfos) {
                    if (m_poolSizeInfos[pair.first] < poolSizeInfos[pair.first]) {
                        isGreater = VG_FALSE;
                        break;
                    }
		        }
            }
		    

			auto oldPool = m_pDescriptorPool;
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
		        			, poolMaxSetCount
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
            auto pBuffer = m_bufferData.getBuffer();
            for (uint32_t i = 0; i < subDataCount; ++i, ++offset)
            {
                auto &subData = m_subDatas[offset];
                subData.init(*(pSubDataInfos + i), pBuffer, m_pDescriptorPool.get());
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

    const BufferData &UniformBufferData::getBufferData() const
    {
        return m_bufferData;
    }

    const vk::DescriptorPool *UniformBufferData::getDescriptorPool() const
    {
        return m_pDescriptorPool.get();
    }

    Bool32 UniformBufferData::_isEqual(uint32_t subDataCount1, const SubData *pSubDatas1, uint32_t subDataOffset1,
            uint32_t subDataCount2, const SubDataInfo *pSubDatas2)
    {
        if (subDataCount1 < subDataOffset1 + subDataCount2) return VG_FALSE;
        for (uint32_t i = 0; i < subDataCount2; ++i)
        {
            const auto &subData1 = *(pSubDatas1 + (subDataOffset1 + i));
            const auto &subData2 = *(pSubDatas2 + i);
            if (subData1.getLayoutBindingCount() != subData2.layoutBindingCount) return VG_FALSE;
            for (uint32_t j = 0; j < subData2.layoutBindingCount; ++j)
            {
                const auto &binding1 = *(subData1.getLayoutBindings() + j);
                const auto &binding2 = *(subData2.pLayoutBindings + j);
                if (binding1 != binding2) return VG_FALSE;
            }
            uint32_t descriptorInfoCount = subData1.getDescriptorInfoCount();
            for (uint32_t j = 0; j < descriptorInfoCount; ++j)
            {
                const auto &info1 = *(subData1.getDescriptorInfos() + j);
                const auto &info2 = *(subData2.pDescriptorInfos + j);
                if (info1 != info2) return VG_FALSE;
            }
            if (subData1.getBufferOffset() != subData2.bufferOffset) return VG_FALSE;
        }
        
        return VG_TRUE;
    }

} //vg