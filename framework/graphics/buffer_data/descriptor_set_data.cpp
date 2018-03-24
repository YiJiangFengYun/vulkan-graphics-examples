#include "graphics/buffer_data/descriptor_set_data.hpp"

#include "graphics/module.hpp"
#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/buffer_data/util.hpp"

namespace vg
{
    DescriptorSetBufferData::DescriptorBufferInfo::DescriptorBufferInfo()
        : type(DescriptorInfoType::BUFFER)
        , range(0u)
        , bufferRange(0u)
    {

    }

    DescriptorSetBufferData::DescriptorBufferInfo::DescriptorBufferInfo(uint32_t range, uint32_t bufferRange)
        : type(DescriptorInfoType::BUFFER)
        , range(range)
        , bufferRange(bufferRange)
    {

    }

    Bool32  DescriptorSetBufferData::DescriptorBufferInfo::operator ==(const DescriptorBufferInfo& target) const
    {
        if (range == target.range && bufferRange == target.bufferRange) return VG_TRUE;
        return VG_FALSE;
    }

	Bool32  DescriptorSetBufferData::DescriptorBufferInfo::operator !=(const DescriptorBufferInfo& target) const
    {
        if ((*this) == target) return VG_FALSE;
        return VG_TRUE;
    }

    DescriptorSetBufferData::DescriptorImageInfo::DescriptorImageInfo()
        : type(DescriptorInfoType::IMAGE)
        , sampler(nullptr)
        , imageView(nullptr)
        , imageLayout(vk::ImageLayout::eUndefined)
    {

    }

    DescriptorSetBufferData::DescriptorImageInfo::DescriptorImageInfo(vk::Sampler sampler
        , vk::ImageView imageView
        , vk::ImageLayout imageLayout
        )
        : type(DescriptorInfoType::IMAGE)
        , sampler(sampler)
        , imageView(imageView)
        , imageLayout(imageLayout)

    {

    }

    Bool32  DescriptorSetBufferData::DescriptorImageInfo::operator ==(const DescriptorImageInfo& target) const
    {
        if (sampler == target.sampler && imageView == target.imageView && imageLayout == target.imageLayout) return VG_TRUE;
        return VG_FALSE;
    }

	Bool32  DescriptorSetBufferData::DescriptorImageInfo::operator !=(const DescriptorImageInfo& target) const
    {
        if ((*this) == target) return VG_FALSE;
        return VG_TRUE;
    }

	DescriptorSetBufferData::DescriptorInfo::DescriptorInfo()
	{
		//bufferInfo = DescriptorBufferInfo();
	}

	/*DescriptorSetBufferData::DescriptorInfo::DescriptorInfo(DescriptorInfoType type)
	{
		if (type == DescriptorInfoType::IMAGE)
		{
			imageInfo = DescriptorImageInfo();
			
		}
		else
		{
			bufferInfo = DescriptorBufferInfo();
		}
	}*/

     Bool32  DescriptorSetBufferData::DescriptorInfo::operator ==(const DescriptorInfo& target) const
    {
        if (this->bufferInfo.type != target.bufferInfo.type) return VG_FALSE;
        if (this->bufferInfo.type == DescriptorInfoType::BUFFER)
        {
            return this->bufferInfo == target.bufferInfo;
        } else if (this->bufferInfo.type == DescriptorInfoType::IMAGE){
            return this->imageInfo == target.imageInfo;
        } else {
            throw std::runtime_error("Unvalid descriptor type.");
        }
    }

	Bool32  DescriptorSetBufferData::DescriptorInfo::operator !=(const DescriptorInfo& target) const
    {
        if ((*this) == target) return VG_FALSE;
        return VG_TRUE;
    }


    DescriptorSetBufferData::SubDataInfo::SubDataInfo()
        : layoutBindingCount(0u)
        , pLayoutBindings(nullptr)
        , pDescriptorInfos(nullptr)
        , bufferOffset(0u)
    {

    }

    DescriptorSetBufferData::SubDataInfo::SubDataInfo(uint32_t layoutBindingCount
        , vk::DescriptorSetLayoutBinding *pLayoutBindings
        , DescriptorInfo *pDescriptorInfos
        , uint32_t bufferOffset
        )
        : layoutBindingCount(layoutBindingCount)
        , pLayoutBindings(pLayoutBindings)
        , pDescriptorInfos(pDescriptorInfos)
        , bufferOffset(bufferOffset)
    {

    }

    DescriptorSetBufferData::SubData::SubData()
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

    DescriptorSetBufferData::SubData::SubData(SubDataInfo info, vk::Buffer *pBuffer, vk::DescriptorPool *pDescriptorPool)
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

    void DescriptorSetBufferData::SubData::init(SubDataInfo info, vk::Buffer *pBuffer, vk::DescriptorPool *pDescriptorPool)
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
            uint32_t bufferInfoCount = 0u;
            uint32_t imageInfoCount = 0u;
            for (uint32_t i = 0; i < m_layoutBindingCount; ++i)
            {
                const auto &binding = m_layoutBindings[i];
                if (binding.descriptorType == vk::DescriptorType::eUniformBuffer)
                {
                    ++bufferInfoCount;
                }
                else if (binding.descriptorType == vk::DescriptorType::eCombinedImageSampler)
                {
                    ++imageInfoCount;
                }
                else 
                {
                    throw std::runtime_error("Unvalid descriptor type.");
                }
            }

		    std::vector<std::vector<vk::DescriptorBufferInfo>> bufferInfoss(bufferInfoCount);
            std::vector<std::vector<vk::DescriptorImageInfo>> imageInfoss(imageInfoCount);
		    uint32_t offset = info.bufferOffset;
		    uint32_t descriptorInfoIndex = 0u;
            uint32_t bufferInfoIndex = 0u;
            uint32_t imageInfoIndex = 0u;
            for (uint32_t bindingIndex = 0u; bindingIndex < m_layoutBindingCount; ++bindingIndex)
            {
                const auto &layoutBinding = m_layoutBindings[bindingIndex];
                if (layoutBinding.descriptorType == vk::DescriptorType::eUniformBuffer)
                {
                    bufferInfoss[bufferInfoIndex].resize(layoutBinding.descriptorCount);
                    for (uint32_t descriptorIndex = 0u; descriptorIndex < layoutBinding.descriptorCount; ++descriptorIndex)
                    {
                        auto &descriptorInfo = m_descriptorInfos[descriptorInfoIndex];
                        bufferInfoss[bufferInfoIndex][descriptorIndex].buffer = buffer;
                        bufferInfoss[bufferInfoIndex][descriptorIndex].offset = offset;
                        bufferInfoss[bufferInfoIndex][descriptorIndex].range = descriptorInfo.bufferInfo.range;
                        offset += descriptorInfo.bufferInfo.bufferRange;
                        ++descriptorInfoIndex;
                    }
                    writes[bindingIndex].dstSet = *m_pDescriptorSet;
		    	    writes[bindingIndex].dstBinding = layoutBinding.binding;
		    	    writes[bindingIndex].descriptorType = layoutBinding.descriptorType;
		    	    writes[bindingIndex].dstArrayElement = 0;
		    	    writes[bindingIndex].descriptorCount = layoutBinding.descriptorCount;
		    	    writes[bindingIndex].pBufferInfo = bufferInfoss[bufferInfoIndex].data();
                    ++bufferInfoIndex;
                    
                }
                else if (layoutBinding.descriptorType == vk::DescriptorType::eCombinedImageSampler)
                {
                    imageInfoss[imageInfoIndex].resize(layoutBinding.descriptorCount);
                    for (uint32_t descriptorIndex = 0u; descriptorIndex < layoutBinding.descriptorCount; ++descriptorIndex)
                    {
                        auto &descriptorInfo = m_descriptorInfos[descriptorInfoIndex];
                        imageInfoss[imageInfoIndex][descriptorIndex].sampler = descriptorInfo.imageInfo.sampler;
                        imageInfoss[imageInfoIndex][descriptorIndex].imageView = descriptorInfo.imageInfo.imageView;
                        imageInfoss[imageInfoIndex][descriptorIndex].imageLayout = descriptorInfo.imageInfo.imageLayout;
                        ++descriptorInfoIndex;
                    }
                    writes[bindingIndex].dstSet = *m_pDescriptorSet;
		    	    writes[bindingIndex].dstBinding = layoutBinding.binding;
		    	    writes[bindingIndex].descriptorType = layoutBinding.descriptorType;
		    	    writes[bindingIndex].dstArrayElement = 0;
		    	    writes[bindingIndex].descriptorCount = layoutBinding.descriptorCount;
		    	    writes[bindingIndex].pImageInfo = imageInfoss[imageInfoIndex].data();
                    ++imageInfoIndex;
                }
                else 
                {
                    throw std::runtime_error("Unvalid descriptor type.");
                }
                
                
            }
		    auto pDevice = pApp->getDevice();
		    pDevice->updateDescriptorSets(writes, nullptr);
        }

        if (bufferChanged) m_buffer = buffer;
    }

    uint32_t DescriptorSetBufferData::SubData::getLayoutBindingCount() const
    {
        return m_layoutBindingCount;
    }

    const vk::DescriptorSetLayoutBinding *DescriptorSetBufferData::SubData::getLayoutBindings() const
    {
        return m_layoutBindings.data();
    }
    
    uint32_t DescriptorSetBufferData::SubData::getDescriptorInfoCount() const
    {
        return m_descriptorInfos.size();
    }
    
    const DescriptorSetBufferData::DescriptorInfo *DescriptorSetBufferData::SubData::getDescriptorInfos() const
    {
        return m_descriptorInfos.data();
    }
            
    uint32_t DescriptorSetBufferData::SubData::getBufferOffset() const
    {
        return m_bufferOffset;
    }
            
    const vk::DescriptorSetLayout *DescriptorSetBufferData::SubData::getDescriptorSetLayout() const
    {
        return m_pDescriptorSetLayout.get();
    }

    const vk::DescriptorSet *DescriptorSetBufferData::SubData::getDescriptorSet() const
    {
        return m_pDescriptorSet.get();
    }

    DescriptorSetBufferData::DescriptorSetBufferData()
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
        //default is host visible.
        if (! m_bufferMemoryPropertyFlags) 
        {
            m_bufferMemoryPropertyFlags |= MemoryPropertyFlagBits::HOST_VISIBLE;
        }
    }
        
    DescriptorSetBufferData::DescriptorSetBufferData(MemoryPropertyFlags bufferMemoryPropertyFlags)
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
        //default is host visible.
        if (! m_bufferMemoryPropertyFlags) 
        {
            m_bufferMemoryPropertyFlags |= MemoryPropertyFlagBits::HOST_VISIBLE;
        }
    }

    DescriptorSetBufferData::~DescriptorSetBufferData()
    {
        if (m_pMemory != nullptr)
        {
            free(m_pMemory);
        }

    }

    void DescriptorSetBufferData::init(uint32_t subDataCount
            , const SubDataInfo *pSubDataInfos
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            )
    {
        updateBuffer(memory, size, cacheMemory);
        updateDesData(subDataCount, pSubDataInfos);
    }

    void DescriptorSetBufferData::updateBuffer(const void *memory
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

    void DescriptorSetBufferData::updateBuffer(fd::ArrayProxy<MemorySlice> memories
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

    void DescriptorSetBufferData::updateSubDataCount(uint32_t count)
    {
        m_subDatas.resize(count);
        m_subDataCount = count;
    }

    void DescriptorSetBufferData::updateDesData(uint32_t subDataCount, const SubDataInfo *pSubDataInfos, uint32_t subDataOffset)
    {
        if (_isEqual(m_subDataCount, m_subDatas.data(), subDataOffset, 
            subDataCount, pSubDataInfos) == VG_FALSE)
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

    uint32_t DescriptorSetBufferData::getSubDataCount() const
    {
        return m_subDataCount;
    }

    const DescriptorSetBufferData::SubData *DescriptorSetBufferData::getSubDatas() const
    {
        return m_subDatas.data();
    }
        
    uint32_t DescriptorSetBufferData::getBufferSize() const
    {
        return m_bufferSize;
    }

    const vk::Buffer *DescriptorSetBufferData::getBuffer() const
    {
        return m_pBuffer.get();
    }

    uint32_t DescriptorSetBufferData::getBufferMemorySize() const
    {
        return m_bufferMemorySize;
    }

    const vk::DeviceMemory *DescriptorSetBufferData::getBufferMemory() const
    {
        return m_pBufferMemory.get();
    }

    uint32_t DescriptorSetBufferData::getMemorySize() const
    {
        return m_memorySize;
    }

    const void *DescriptorSetBufferData::getMemory() const
    {
        return m_pMemory;
    }

    const vk::DescriptorPool *DescriptorSetBufferData::getDescriptorPool() const
    {
        return m_pDescriptorPool.get();
    }

    Bool32 DescriptorSetBufferData::_isDeviceMemoryLocal() const
    {
        return (m_bufferMemoryPropertyFlags & MemoryPropertyFlagBits::DEVICE_LOCAL) == MemoryPropertyFlagBits::DEVICE_LOCAL;
    }

    void DescriptorSetBufferData::_createBuffer(fd::ArrayProxy<MemorySlice> memories, uint32_t memorySize)
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

    Bool32 DescriptorSetBufferData::_isEqual(uint32_t subDataCount1, const SubData *pSubDatas1, uint32_t subDataOffset1,
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