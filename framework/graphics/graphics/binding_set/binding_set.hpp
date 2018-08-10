#ifndef VG_BINDING_SET_HPP
#define VG_BINDING_SET_HPP

#include "graphics/global.hpp"
#include "graphics/binding_set/binding_set_data.hpp"

namespace vg
{
    class BindingSet {
    public:

        BindingSet();

        Bool32 hasData(std::string name) const;
        void removeData(std::string name);

        void addData(const std::string name, const BindingSetDataInfo &info, const BindingSetDataSizeInfo &sizeInfo);
        void addData(const std::string name, const BindingSetDataInfo &info, const void *src, uint32_t size);
        void getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const;
        void setData(const std::string name, const BindingSetDataInfo &info, const BindingSetDataSizeInfo &sizeInfo);
        void setData(const std::string name, const void *src, uint32_t size, uint32_t offset);

        template<typename T>
        void addData(const std::string name, const BindingSetDataInfo &info, const T &value);
        template<typename T>
        T getData(const std::string name) const;
        template<typename T>
        void setData(const std::string name, const T &value);
        
        template<typename T>
        void addData(const std::string name, const BindingSetDataInfo &info, const std::vector<T> &values);
        template <typename T>
        std::vector<T> getData(const std::string name, const uint32_t count) const;
        template<typename T>
        void setData(const std::string name, const std::vector<T> &values);

        template<typename T>
        void addData(const std::string name, const BindingSetDataInfo &info, const T * const pSrc, const uint32_t count);
        template<typename T>
        void getData(const std::string name, const T * const pDst, const uint32_t count);
        template<typename T>
        void setData(const std::string name, const T * const pSrc, const uint32_t count);

        Bool32 hasBuffer(std::string name) const;
        void addBuffer(std::string name, const BindingSetBufferInfo &bufferInfo);
        void removeBuffer(std::string name);
        BindingSetBufferInfo getBuffer(std::string name);
        void setBuffer(std::string name, const BindingSetBufferInfo &bufferInfo);

        Bool32 hasTexture(std::string name) const;
        void addTexture(std::string name, const BindingSetTextureInfo &texInfo);
        void removeTexture(std::string name);
        BindingSetTextureInfo getTexture(std::string name) const;
        void setTexture(std::string name, const BindingSetTextureInfo &texInfo);

        const BufferData &getBufferData() const;
        const vk::DescriptorSetLayout *getDescriptorSetLayout() const;
        const vk::DescriptorPool *getDescriptorPool() const;
        const vk::DescriptorSet *getDescriptorSet() const;

        void apply();

    private:
        BindingSetData m_data;
        Bool32 m_dataChanged;
        Bool32 m_dataContentChanged;
        std::unordered_map<std::string, Bool32> m_dataContentChanges;
        Bool32 m_textureChanged;
        Bool32 m_bufferChanged;

        ////////applied data
        struct DataSortInfo {
            std::string name;
            vk::ShaderStageFlags shaderStageFlags;
            uint32_t layoutPriority;
            uint32_t size;
            uint32_t bufferSize;

            DataSortInfo(std::string name = nullptr
                , vk::ShaderStageFlags shaderStageFlags = vk::ShaderStageFlags()
                , uint32_t layoutPriority = 0u
                , uint32_t size = 0u
                , uint32_t bufferSize = 0u
                );
        };
        static Bool32 _compareDataInfo(const DataSortInfo &, const DataSortInfo &);
        std::set<DataSortInfo, Bool32(*)(const DataSortInfo &, const DataSortInfo &)> m_sortDataSet;
        BufferData m_dataBuffer;

        struct BufferTextureSortInfo {
            std::string name;
            uint32_t bindingPriority;
            Bool32 isTexture;
            const void *pData;

            BufferTextureSortInfo(std::string name = nullptr
                , uint32_t bindingPriority = 0u
                , Bool32 isTexture = VG_FALSE
                , const void *pData = nullptr
                );
        };
        static Bool32 _compareBufferTextureInfo(const BufferTextureSortInfo &, const BufferTextureSortInfo &);
        std::set<BufferTextureSortInfo, Bool32(*)(const BufferTextureSortInfo &, const BufferTextureSortInfo &)> m_sortBufferTexInfosSet;
        Bool32 m_descriptorSetChanged;
        uint32_t m_layoutBindingCount;
        std::vector<vk::DescriptorSetLayoutBinding> m_descriptorSetLayoutBindings;
        struct UpdateDescriptorSetInfo {
            std::vector<vk::DescriptorBufferInfo> bufferInfos;
            std::vector<vk::DescriptorImageInfo> imageInfos;

            UpdateDescriptorSetInfo(std::vector<vk::DescriptorBufferInfo> bufferInfos = std::vector<vk::DescriptorBufferInfo>()
                , std::vector<vk::DescriptorImageInfo> imageInfos = std::vector<vk::DescriptorImageInfo>()
                );
        };
        std::vector<UpdateDescriptorSetInfo> m_updateDescriptorSetInfos;
        std::shared_ptr<vk::DescriptorSetLayout> m_pDescriptorSetLayout;

        //build in descriptor pool.
        std::unordered_map<vk::DescriptorType, uint32_t> m_poolSizeInfos;
        std::shared_ptr<vk::DescriptorPool> m_pDescriptorPool;

        //build in descriptor set
        std::shared_ptr<vk::DescriptorSet> m_pDescriptorSet;
    };
} //vg

#endif //VG_BINDING_SET_HPP