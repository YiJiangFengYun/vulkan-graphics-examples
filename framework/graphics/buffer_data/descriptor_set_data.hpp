#ifndef VG_DESCRIPTOR_SET_DATA_HPP
#define VG_DESCRIPTOR_SET_DATA_HPP

#include <foundation/foundation.hpp>
#include "graphics/global.hpp"
#include "graphics/buffer_data/buffer_data_option.hpp"

namespace vg
{
    //One subData is equal to one descriptorSet.    
    class DescriptorSetBufferData : public Base
    {
    public:
        enum class DescriptorInfoType
        {
            BUFFER,
            IMAGE
        };

        struct DescriptorBufferInfo 
        {
            DescriptorInfoType type;
            uint32_t range;
            uint32_t bufferRange;
            DescriptorBufferInfo();
            DescriptorBufferInfo(uint32_t range, uint32_t bufferRange);

            Bool32 operator ==(const DescriptorBufferInfo& target) const;
			Bool32 operator !=(const DescriptorBufferInfo& target) const;
        };

        struct DescriptorImageInfo 
        {
            DescriptorInfoType type;
            vk::Sampler sampler;
            vk::ImageView imageView;
            vk::ImageLayout imageLayout;
            DescriptorImageInfo();
            DescriptorImageInfo(vk::Sampler sampler
                , vk::ImageView imageView
                , vk::ImageLayout imageLayout);
            Bool32 operator ==(const DescriptorImageInfo& target) const;
			Bool32 operator !=(const DescriptorImageInfo& target) const;
        };

        union DescriptorInfo
        {
		public:
            DescriptorBufferInfo bufferInfo;
            DescriptorImageInfo imageInfo;

			DescriptorInfo();
			//DescriptorInfo(DescriptorInfoType type);

            Bool32 operator ==(const DescriptorInfo& target) const;
			Bool32 operator !=(const DescriptorInfo& target) const;

		private:
			
        };

        struct SubDataInfo 
        {
            uint32_t layoutBindingCount;
            vk::DescriptorSetLayoutBinding *pLayoutBindings;
            //Sum of buffer info count and image info count must is equal to descriptor count;
            //Infos of same binding must continue in the one infos array.
            DescriptorInfo *pDescriptorInfos;
            // DescriptorBufferInfo *pDescriptorInfos;
            uint32_t bufferOffset;

            SubDataInfo();
            SubDataInfo(uint32_t layoutBindingCount = 0u
                , vk::DescriptorSetLayoutBinding *pLayoutBindings = nullptr
                , DescriptorInfo *pDescriptorInfos = nullptr
                , uint32_t bufferOffset = 0u);
        };

        struct SubData
        {
        public:
            SubData();
            SubData(SubDataInfo info, vk::Buffer *pBuffer, vk::DescriptorPool *pDescriptorPool);
            
            void init(SubDataInfo info, vk::Buffer *pBuffer, vk::DescriptorPool *pDescriptorPool);

            uint32_t getLayoutBindingCount() const;
            const vk::DescriptorSetLayoutBinding *getLayoutBindings() const;
            uint32_t getDescriptorInfoCount() const;
            const DescriptorInfo *getDescriptorInfos() const;
            uint32_t getBufferOffset() const;
            const vk::DescriptorSetLayout *getDescriptorSetLayout() const;
            const vk::DescriptorSet *getDescriptorSet() const;
        private:
            uint32_t m_layoutBindingCount;
            std::vector<vk::DescriptorSetLayoutBinding> m_layoutBindings;
            std::vector<DescriptorInfo> m_descriptorInfos;
            uint32_t m_bufferOffset;
            std::shared_ptr<vk::DescriptorSetLayout> m_pDescriptorSetLayout;
            std::shared_ptr<vk::DescriptorSet> m_pDescriptorSet;
            vk::DescriptorPool m_descriptorPool;
            vk::Buffer m_buffer;
        };

        DescriptorSetBufferData();
        DescriptorSetBufferData(MemoryPropertyFlags bufferMemoryPropertyFlags);

        ~DescriptorSetBufferData();

        void init(uint32_t subDataCount
            , const SubDataInfo *pSubDataInfos
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            );

        void updateBuffer(const void *memory
            , uint32_t size
            , Bool32 cacheMemory
            );

        void updateBuffer(fd::ArrayProxy<MemorySlice> memories
            , uint32_t size
            , Bool32 cacheMemory
            );

        void updateSubDataCount(uint32_t count);

        void updateDesData(uint32_t subDataCount, const SubDataInfo *pSubDataInfos, uint32_t subDataOffset = 0u);

        uint32_t getSubDataCount() const;
        const SubData *getSubDatas() const;
        uint32_t getBufferSize() const;
        const vk::Buffer *getBuffer() const;
        uint32_t getBufferMemorySize() const;
        const vk::DeviceMemory *getBufferMemory() const;
        uint32_t getMemorySize() const;
        const void *getMemory() const;
        const vk::DescriptorPool *getDescriptorPool() const;
    private:
        uint32_t m_subDataCount;
        std::vector<SubData> m_subDatas;
        uint32_t m_bufferSize;
        std::shared_ptr<vk::Buffer> m_pBuffer;
        uint32_t m_bufferMemorySize;
        std::shared_ptr<vk::DeviceMemory> m_pBufferMemory;
        MemoryPropertyFlags m_bufferMemoryPropertyFlags;
        uint32_t m_memorySize;
        void *m_pMemory;
        void *m_pMmemoryForHostVisible;

        std::unordered_map<vk::DescriptorType, uint32_t> m_poolSizeInfos;
        std::shared_ptr<vk::DescriptorPool> m_pDescriptorPool;

        Bool32 _isDeviceMemoryLocal() const;
        void _createBuffer(fd::ArrayProxy<MemorySlice> memories, uint32_t memorySize);
        Bool32 _isEqual(uint32_t subDataCount1, const SubData *pSubDatas1, uint32_t subDataOffset1,
            uint32_t subDataCount2, const SubDataInfo *pSubDatas2);

    };
} //vg

#endif //VG_DESCRIPTOR_SET_DATA_HPP