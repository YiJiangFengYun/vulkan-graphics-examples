#ifndef VG_UNIFORM_BUFFER_DATA_HPP
#define VG_UNIFORM_BUFFER_DATA_HPP

#include <foundation/foundation.hpp>
#include "graphics/global.hpp"
#include "graphics/buffer_data/buffer_data_option.hpp"

namespace vg
{
    //One subData is equal to one descriptorSet.    
    class UniformBufferData : public Base
    {
    public:
        struct DescriptorBufferInfo 
        {
            uint32_t range;
            uint32_t bufferRange;
            uint32_t dynamicOffset;
            DescriptorBufferInfo(uint32_t range = 0u, uint32_t bufferRange = 0u, uint32_t dynamicOffset = 0u);

            Bool32 operator ==(const DescriptorBufferInfo& target) const;
			Bool32 operator !=(const DescriptorBufferInfo& target) const;
        };

        struct SubDataInfo 
        {
            uint32_t layoutBindingCount;
            vk::DescriptorSetLayoutBinding *pLayoutBindings;
            //Sum of buffer info count and image info count must is equal to descriptor count;
            //Infos of same binding must continue in the one infos array.
            DescriptorBufferInfo *pDescriptorInfos;
            uint32_t bufferOffset;

            SubDataInfo(uint32_t layoutBindingCount = 0u
                , vk::DescriptorSetLayoutBinding *pLayoutBindings = nullptr
                , DescriptorBufferInfo *pDescriptorInfos = nullptr
                , uint32_t bufferOffset = 0u);
        };

        struct SubData
        {
        public:
            SubData();
            SubData(SubDataInfo info, std::shared_ptr<vk::Buffer> pBuffer, std::shared_ptr<vk::DescriptorPool> pDescriptorPool);
			~SubData();
            void init(SubDataInfo info, std::shared_ptr<vk::Buffer> pBuffer, std::shared_ptr<vk::DescriptorPool> pDescriptorPool);

            uint32_t getLayoutBindingCount() const;
            const vk::DescriptorSetLayoutBinding *getLayoutBindings() const;
            uint32_t getDescriptorInfoCount() const;
            const DescriptorBufferInfo *getDescriptorInfos() const;
            uint32_t getBufferOffset() const;
            const vk::DescriptorSetLayout *getDescriptorSetLayout() const;
            const vk::DescriptorSet *getDescriptorSet() const;

        private:
            uint32_t m_layoutBindingCount;
            std::vector<vk::DescriptorSetLayoutBinding> m_layoutBindings;
            std::vector<DescriptorBufferInfo> m_descriptorInfos;
            uint32_t m_bufferOffset;
            std::shared_ptr<vk::DescriptorSetLayout> m_pDescriptorSetLayout;
            std::shared_ptr<vk::DescriptorSet> m_pDescriptorSet;
            std::shared_ptr<vk::DescriptorPool> m_pDescriptorPool;
            std::shared_ptr<vk::Buffer> m_pBuffer;
        };

        UniformBufferData();
        UniformBufferData(MemoryPropertyFlags bufferMemoryPropertyFlags);

        ~UniformBufferData();

        void init(uint32_t subDataCount
            , const SubDataInfo *pSubDataInfos
            , const void *memory
            , uint32_t size
            , Bool32 cacheMemory = VG_FALSE
            );

        void updateBuffer(const void *memory
            , uint32_t size
            , Bool32 cacheMemory = VG_FALSE
            );

        void updateBuffer(fd::ArrayProxy<MemorySlice> memories
            , uint32_t size
            , Bool32 cacheMemory = VG_FALSE
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
        uint32_t m_bufferSize;
        std::shared_ptr<vk::Buffer> m_pBuffer;
        uint32_t m_bufferMemorySize;
        std::shared_ptr<vk::DeviceMemory> m_pBufferMemory;
        MemoryPropertyFlags m_bufferMemoryPropertyFlags;
        uint32_t m_memorySize;
        void *m_pMemory;
        void *m_pMmemoryForHostVisible;

        std::uint32_t m_poolMaxSetCount;
        std::unordered_map<vk::DescriptorType, uint32_t> m_poolSizeInfos;
		std::shared_ptr<vk::DescriptorPool> m_pDescriptorPool;


		uint32_t m_subDataCount;
		std::vector<SubData> m_subDatas;
        

        Bool32 _isDeviceMemoryLocal() const;
        void _createBuffer(fd::ArrayProxy<MemorySlice> memories, uint32_t memorySize);
        Bool32 _isEqual(uint32_t subDataCount1, const SubData *pSubDatas1, uint32_t subDataOffset1,
            uint32_t subDataCount2, const SubDataInfo *pSubDatas2);

    };
} //vg

#endif //VG_UNIFORM_BUFFER_DATA_HPP